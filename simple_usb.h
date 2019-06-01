/**
 * @file simple_usb.h
 */
#ifndef SIMPLE_USB_H
#define SIMPLE_USB_H

/** 正常終了 */
#define SUSB_SUCCESS   (0)
/** 出力用のエンドポイントのエラー */
#define SUSB_ERR_EP_O  (-1)
/** 入力用のエンドポイントのエラー */
#define SUSB_ERR_EP_I  (-2)
/** USB転送時のタイムアウト時間 (ms) */
#define SUSB_TIMEOUT   (5000)


typedef struct simple_usb simple_usb;

simple_usb *susb_open(int vendor, int product, int interface);
int susb_close(simple_usb *su);
int susb_interrupt_sr(simple_usb *su,
		int ep_out, unsigned char *sbuf, int slen,
		int ep_in, unsigned char *rbuf, int rlen);


#endif

