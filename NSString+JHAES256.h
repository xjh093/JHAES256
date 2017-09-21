//
//  NSString+AES256.h
//  JHKit
//
//  Created by HaoCold on 2017/2/27.
//  Copyright © 2017年 HaoCold. All rights reserved.
//

#import <Foundation/Foundation.h>

/**< iOS,Android,PHP*/
/**< 根据后台给定的具体值，进行修改。*/
#define AES_IV @"5efd3f6060e20330"

@interface NSString (JHAES256)

-(NSString *)jh_aes256_encrypt:(NSString *)key; //加密
-(NSString *)jh_aes256_decrypt:(NSString *)key; //解密

@end

@interface NSData (JHAES256)

- (NSData *)jh_aes256_encrypt:(NSString *)key; //加密
- (NSData *)jh_aes256_decrypt:(NSString *)key; //解密

@end
