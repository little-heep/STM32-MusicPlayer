/*  Copyright (s) 2019 深圳百问网科技有限公司
 *  All rights reserved
 * 
 * 文件名称：driver_oled.h
 * 摘要：
 *  
 * 修改历史     版本号        Author       修改内容
 *--------------------------------------------------
 * 2020.8.18      v01         百问科技      创建文件
 *--------------------------------------------------
*/

#ifndef __DRIVER_SYSTICK_H
#define __DRIVER_SYSTICK_H

#include <stdint.h>

void udelay(int us);

void mdelay(int ms);

uint64_t system_get_ns(void);
#endif /* __DRIVER_SYSTICK_H */

