#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "error.h"
#include "operate.h"

static void help(void);
static void get_option(struct Options_ *opt, int argc, char *argv[]);

static char *Usage =
"usage: wrdctl\n"
"\n"
"WRD-130-UIとの通信を行う\n"
"設定によっては管理者権限が必要\n"
"オプション無しの場合、USBの接続、切断のみ行う\n"
"\n"
"optional arguments:\n"
"  -h       show this help message and exit\n"
"  -C str   strをそのまま送信する (通信に必要なヘッダは付ける)\n"
"  -T       転送確認\n"
"\n";


int main(int argc, char *argv[])
{
	struct Options_ opt;

	// オプション解析
	get_option(&opt, argc, argv);

	return operate(&opt);
}

static void help()
{
	printf("%s", Usage);
}

/*
 * オプション解析
 */
static void get_option(struct Options_ *opt, int argc, char *argv[])
{
	int c;

	// 初期化
	opt->action = ACT_NONE;
	opt->str = NULL;

	while ((c = getopt(argc, argv, "hTC:")) != -1) {
		switch (c) {
		case 'h':
			help();
			exit(0);
		case 'T':
			opt->action = ACT_TRANSFER_TEST;
			break;
		case 'C':
			opt->action = ACT_RAW_CMD;
			opt->str = optarg;
			break;
		default:
			help();
			exit(1);
		}
	}

	// オプション以外の処理
	//char *str = argv[optind];
}

