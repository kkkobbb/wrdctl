#ifndef OPERATE_H
#define OPERATE_H


enum action_type {
	ACT_NONE,
	ACT_TRANSFER_TEST,
	ACT_RAW_CMD,

	ACT_READ_SINGLE_ID,
	ACT_READ_SINGLE_DATA,
	ACT_READ_MULTI_ID,
	ACT_READ_MULTI_DATA,
	ACT_READ_FILTER_ID,
	ACT_READ_FILTER_DATA,
	ACT_WRITE_SINGLE_ID,
	ACT_WRITE_SINGLE_DATA,
	ACT_WRITE_MULTI_ID,
	ACT_WRITE_MULTI_DATA,
	ACT_WRITE_FILTER_ID,
	ACT_WRITE_FILTER_DATA,
};


struct Options_ {
	int action;
	char *str;
};


int operate(struct Options_ *opt);


#endif


