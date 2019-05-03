/*
 *  linux/kernel/vsprintf.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

/* vsprintf.c -- Lars Wirzenius & Linus Torvalds. */
/*
 * Wirzenius wrote this portably, Torvalds fucked it up :-)
 */

#include <stdarg.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/ctype.h>

unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base)  //将一个字符串转换成unsigend long型数据 
{
	unsigned long result = 0,value;

	if (!base) {
		base = 10;
		if (*cp == '0') {
			base = 8;
			cp++;
			if ((*cp == 'x') && isxdigit(cp[1])) {
				cp++;
				base = 16;
			}
		}
	}
	while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp-'0' : (islower(*cp)
	    ? toupper(*cp) : *cp)-'A'+10) < base) {
		result = result*base + value;
		cp++;
	}
	if (endp)
		*endp = (char *)cp;
	return result;
}

/* we use this so that we can do without the ctype library */
#define is_digit(c)	((c) >= '0' && (c) <= '9')              //is_digit(c)用来判断c是不是0-9的字符

static int skip_atoi(const char **s)                        //skip_atoi用来将传入的数字字符串转化为int类型 
{
	int i=0;

	while (is_digit(**s))              //如果*s指向的是0-9数字，则一直循环
		i = i*10 + *((*s)++) - '0';    //*s指向字符的值减去0的ASCII码就是字符本身表示的数字；i*10是为了保存上次计算出的数字；
	return i;
}

#define ZEROPAD	1		/* pad with zero */
#define SIGN	2		/* unsigned/signed long */
#define PLUS	4		/* show plus */
#define SPACE	8		/* space if plus */
#define LEFT	16		/* left justified */
#define SPECIAL	32		/* 0x */
#define SMALL	64		/* use 'abcdef' instead of 'ABCDEF' */

#define do_div(n,base) ({ \
int __res; \
__asm__("divl %4":"=a" (n),"=d" (__res):"0" (n),"1" (0),"r" (base)); \
__res; })

static char * number(char * str, int num, int base, int size, int precision
	,int type)
{
	char c,sign,tmp[36];
	const char *digits="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int i;

	if (type&SMALL) digits="0123456789abcdefghijklmnopqrstuvwxyz";
	if (type&LEFT) type &= ~ZEROPAD;
	if (base<2 || base>36)
		return 0;
	c = (type & ZEROPAD) ? '0' : ' ' ;
	if (type&SIGN && num<0) {
		sign='-';
		num = -num;
	} else
		sign=(type&PLUS) ? '+' : ((type&SPACE) ? ' ' : 0);
	if (sign) size--;
	if (type&SPECIAL)
		if (base==16) size -= 2;
		else if (base==8) size--;
	i=0;
	if (num==0)
		tmp[i++]='0';
	else while (num!=0)
		tmp[i++]=digits[do_div(num,base)];
	if (i>precision) precision=i;
	size -= precision;
	if (!(type&(ZEROPAD+LEFT)))
		while(size-->0)
			*str++ = ' ';
	if (sign)
		*str++ = sign;
	if (type&SPECIAL)
		if (base==8)
			*str++ = '0';
		else if (base==16) {
			*str++ = '0';
			*str++ = digits[33];
		}
	if (!(type&LEFT))
		while(size-->0)
			*str++ = c;
	while(i<precision--)
		*str++ = '0';
	while(i-->0)
		*str++ = tmp[i];
	while(size-->0)
		*str++ = ' ';
	return str;
}

int vsprintf(char *buf, const char *fmt, va_list args)//fmt是格式说明符号，buf是待存入数据的buffer
{
	int len;
	int i;
	char * str;
	char *s;
	int *ip;

	int flags;		/* flags to number() */

	int field_width;	/* width of output field */
	int precision;		/* min. # of digits for integers; max
				   number of chars for from string */
	int qualifier;		/* 'h', 'l', or 'L' for integer fields */

	for (str=buf ; *fmt ; ++fmt) {  //扫描格式说明符
		if (*fmt != '%') { //如果没有格式说明符号，则直接写入str指向的buf
			*str++ = *fmt;
			continue;      //continue相当于实现一个循环，知道遇到格式说明符%；
		}
        
		//走到这就说明碰到了格式说明符%	
		/* process flags */
		flags = 0;
		repeat:
			++fmt;		/* this also skips first '%' */
			switch (*fmt) {   //取格式说明符%后的一个字符，如果是 - + 空格 # 0 的情况
				case '-': flags |= LEFT; goto repeat;  //左对齐
				case '+': flags |= PLUS; goto repeat;  //右对齐
				case ' ': flags |= SPACE; goto repeat;  //
				case '#': flags |= SPECIAL; goto repeat;  //
				case '0': flags |= ZEROPAD; goto repeat;  //
				}
		
		/* get field width */
		field_width = -1;     //格式输出宽度
		if (is_digit(*fmt))   //如果是个数字字符
			field_width = skip_atoi(&fmt);//把格式输出宽度由字符改为数字，char改为int
		else if (*fmt == '*') {           //如果是个"*"
			/* it's the next argument */
			field_width = va_arg(args, int);
			if (field_width < 0) {        //如果参数是个负数，那么左对齐
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		/* get the precision */
		precision = -1;
		if (*fmt == '.') {             //如果格式说明符%后跟了".",说明对输出精度有要求
			++fmt;	                   //跳过'.'
			if (is_digit(*fmt))
				precision = skip_atoi(&fmt);
			else if (*fmt == '*') {    //如果是*，取args参数作为精度要求
				/* it's the next argument */
				precision = va_arg(args, int);
			}
			if (precision < 0)         //要求精度必须大于0
				precision = 0;
		}

		/* get the conversion qualifier */
		qualifier = -1;
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L') {
			qualifier = *fmt;
			++fmt;
		}

		switch (*fmt) {
		case 'c':                      //如果后面跟c，把此时的参数当ASCII码对待，说明将参数以字符形式打印出来
			if (!(flags & LEFT))       //没有左对齐，进入if判断
				while (--field_width > 0)
					*str++ = ' ';
			*str++ = (unsigned char) va_arg(args, int); //把对应的参数写入str指定的buffer
			while (--field_width > 0)                   //对格式输出宽度有要求时，用空格补齐
				*str++ = ' ';
			break;

		case 's':                   //字符串
			s = va_arg(args, char *);
			if (!s)
				s = "<NULL>";
			len = strlen(s);
			if (precision < 0)
				precision = len;
			else if (len > precision)
				len = precision;

			if (!(flags & LEFT))
				while (len < field_width--)
					*str++ = ' ';
			for (i = 0; i < len; ++i)
				*str++ = *s++;      //把s指向的字符串一个个copy到str指向的buffer中
			while (len < field_width--)
				*str++ = ' ';
			break;

		case 'o':   //八进制输出
			str = number(str, va_arg(args, unsigned long), 8,
				field_width, precision, flags);
			break;

		case 'p':   //指针输出
			if (field_width == -1) {
				field_width = 8;
				flags |= ZEROPAD;
			}
			str = number(str,
				(unsigned long) va_arg(args, void *), 16,
				field_width, precision, flags);
			break;

		case 'x':   //十六进制输出 小写
			flags |= SMALL;
		case 'X':   //十六进制输出 大写
			str = number(str, va_arg(args, unsigned long), 16,
				field_width, precision, flags);
			break;

		case 'd':   //有符号整形输出
		case 'i':
			flags |= SIGN;
		case 'u':   //无符号整形输出
			str = number(str, va_arg(args, unsigned long), 10,
				field_width, precision, flags);
			break;

		case 'n':
			ip = va_arg(args, int *);
			*ip = (str - buf);
			break;

		default:
			if (*fmt != '%')
				*str++ = '%';
			if (*fmt)
				*str++ = *fmt;
			else
				--fmt;
			break;
		}
	}
	*str = '\0';  //空字符结尾
	return str-buf;
}

int sprintf(char * buf, const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i=vsprintf(buf,fmt,args);
	va_end(args);
	return i;
}

