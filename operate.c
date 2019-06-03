#include <stdio.h>
#include <string.h>

#include "operate.h"
#include "simple_usb.h"
#include "error.h"

#define WRD_VENDOR_ID 0x0e6a
#define WRD_PRODUCT_ID 0x0317
#define WRD_INTERFACE 1
#define WRD_ENDPOINT_OUT 0x03
#define WRD_ENDPOINT_IN 0x82

#define WRD_TRANS_SBUF_SIZE 64
#define WRD_TRANS_RBUF_SIZE 1024
#define WRD_TRANS_HEADER_SIZE 3


static void print_binary(unsigned char *bin, int size)
{
	int i;
	for (i = 0; i < size; i++) {
		printf("%02x ", bin[i]);
		if ((i + 1) % 16 == 0)
			printf("\n");
	}

	if (size % 16 != 0)
		printf("\n");
}

/*
 * コマンド送受信
 *
 * strを送信する
 */
static void cmd_raw(simple_usb *su, char *str, int str_len)
{
	unsigned char sbuf[WRD_TRANS_SBUF_SIZE] = {0x0a, 0x02, 0x41, 0};  // 送信用ヘッダ付加
	unsigned char rbuf[WRD_TRANS_RBUF_SIZE] = {0};
	int cpy_size = sizeof(sbuf) - WRD_TRANS_HEADER_SIZE;
	int send_size = 0;
	int recv_size = 0;

	// 送信データの設定
	if (0 < str_len && str_len < cpy_size)
		cpy_size = str_len;
	memcpy(sbuf + WRD_TRANS_HEADER_SIZE, str, cpy_size);
	send_size = WRD_TRANS_HEADER_SIZE + cpy_size;

	// 送信データ表示
	printf("\n# Send: %d\n", send_size);
	print_binary(sbuf, send_size);

	recv_size = susb_interrupt_sr(su,
			WRD_ENDPOINT_OUT, sbuf, send_size,
			WRD_ENDPOINT_IN, rbuf, sizeof(rbuf));
	if (recv_size < 0) {
		error_only("transfer usb %d", recv_size);
		return;
	}

	// 受信データ表示
	printf("\n# Recv: %d\n", recv_size);
	print_binary(rbuf, recv_size);
}

/*
 * WRD用のコマンド送信＆結果取得 確認用
 */
static void cmd_test(simple_usb *su)
{
	//char *str = "R2,0,C";  // データ読み込み
	//char *str = "Q";  // ID読み込み
	char *str = "V";

	cmd_raw(su, str, strlen(str));
}

/*
 * USB制御
 *
 * エラー時、非ゼロを返す
 */
int operate(struct Options_ *opt)
{
	int ret = 0;
	simple_usb *su;

	// usbデバイスオープン
	su = susb_open(WRD_VENDOR_ID, WRD_PRODUCT_ID, WRD_INTERFACE);
	if (su == NULL) {
		error_only("usb open");
		return 1;
	}

	switch (opt->action) {
	case ACT_NONE:
		// 何もしない
		break;
	case ACT_TRANSFER_TEST:
		// USB転送確認
		cmd_test(su);
		break;
	case ACT_RAW_CMD:
		// データ送受信
		cmd_raw(su, opt->str, strlen(opt->str));
		break;
	default:
		error_only("unknown action");
		ret = 1;
	}

	// usbデバイスクローズ
	susb_close(su);

	return ret;
}

