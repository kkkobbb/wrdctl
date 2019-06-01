/**
 * 簡易USB通信
 */
#include <stdio.h>
#include <stdlib.h>

#include <libusb-1.0/libusb.h>

#include "simple_usb.h"

/**
 * USB通信用情報
 * @private
 */
struct simple_usb {
	libusb_context *ctx;
	libusb_device_handle *dh;
	int interface;     /**< 使用するインターフェースの番号 */
	int need_release;  /**< interfaceの解放が必要なら真 */
	int need_attach;   /**< interfaceのアタッチが必要なら真 */
};


/**
 * USB通信のための初期化
 * @param vendor 対象USB機器のベンダーID
 * @param product 対象USB機器のプロダクトID
 * @param interface 通信するインターフェース番号
 * @return 生成したUSB通信用情報
 */
simple_usb *susb_open(int vendor, int product, int interface)
{
	int r;
	simple_usb *su = NULL;

	su = malloc(sizeof(*su));
	if (su == NULL)
		return NULL;

	su->interface = interface;
	su->need_release = 0;
	su->need_attach = 0;

	r = libusb_init(&su->ctx);
	if (r < 0) {
		// Error: init usb
		free(su);
		return NULL;
	}

	//libusb_set_debug(su->ctx, 4);

	su->dh = libusb_open_device_with_vid_pid(su->ctx, vendor, product);
	if (su->dh == NULL) {
		// Error: open device
		libusb_exit(su->ctx);
		free(su);
		return NULL;
	}

	// カーネルがデバイスをアクティブにしているとlibusbで使えないので
	// カーネルでの使用を無効にする
	if (libusb_kernel_driver_active(su->dh, interface) == 1) {
		// kernel driver active
		su->need_attach = 1;
		libusb_detach_kernel_driver(su->dh, interface);
	}

	// インターフェースの指定
	r = libusb_claim_interface(su->dh, interface);
	if (r < 0) {
		// Error: claim interface
		susb_close(su);
		return NULL;
	}
	su->need_release = 1;

	return su;
}

/**
 * USB通信の終了処理
 * @param su 終了するUSB通信用情報
 * @return 成功時、ゼロ
 */
int susb_close(simple_usb *su)
{
	if (su == NULL)
		return SUSB_SUCCESS;

	if (su->need_release)
		libusb_release_interface(su->dh, su->interface);
	if (su->need_attach)
		libusb_attach_kernel_driver(su->dh, su->interface);
	libusb_close(su->dh);
	libusb_exit(su->ctx);

	free(su);

	return SUSB_SUCCESS;
}

/**
 * interrupt転送 送信&受信
 * @param su USB通信用情報
 * @param ep_out 送信(OUT)側のエンドポイント番号
 * @param sbuf 送信するデータ
 * @param slen 送信するデータサイズ
 * @param ep_in 受信(IN)側のエンドポイント番号
 * @param rbuf 受信バッファ
 * @param rlen 受信バッファのサイズ
 * @return 負数:エラー 0以上:受信サイズ
 */
int susb_interrupt_sr(simple_usb *su,
		int ep_out, unsigned char *sbuf, int slen,
		int ep_in, unsigned char *rbuf, int rlen)
{
	int r;  // libusbの実行結果
	int t;  // 転送したバイト数

	if (ep_out >= 0) {
		// 送信処理
		r = libusb_interrupt_transfer(su->dh, ep_out, sbuf, slen, &t, SUSB_TIMEOUT);
		//printf("# send ret:%d transferred:%d\n", r, t);
		if (r < 0)
			return SUSB_ERR_EP_O;
	}

	if (ep_in >= 0) {
		// 受信処理
		r = libusb_interrupt_transfer(su->dh, ep_in, rbuf, rlen, &t, SUSB_TIMEOUT);
		//printf("# recv ret:%d transferred:%d\n", r, t);
		if (r < 0)
			return SUSB_ERR_EP_I;
	}

	return t;
}

