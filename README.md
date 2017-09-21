# NSString-JHAES256


.h
```
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
```


.m

```
#import "NSString+JHAES256.h"
#import <CommonCrypto/CommonCryptor.h>

@implementation NSString (JHAES256)

-(NSString *)jh_aes256_encrypt:(NSString *)key
{
#if 0
    /**< 中文长度 与 英文长度 计算不同，一个中文占3个字节
     
     self 为 "瞅你咋地，就抽你，呃呃呃"
     
     const char *cstr = [self cStringUsingEncoding:NSUTF8StringEncoding];
     NSData *data = [NSData dataWithBytes:cstr length:self.length];
     
     data.length 等于 12
     
     NSData *data = [self dataUsingEncoding:NSUTF8StringEncoding];
     
     data.length 等于 36
     
     */
    const char *cstr = [self cStringUsingEncoding:NSUTF8StringEncoding];
    NSData *data = [NSData dataWithBytes:cstr length:self.length];
#else
    NSData *data = [self dataUsingEncoding:NSUTF8StringEncoding];
#endif
    //对数据进行加密
    NSData *result = [data jh_aes256_encrypt:key];
    
    //转换为2进制字符串
    if (result && result.length > 0) {
        
        Byte *datas = (Byte*)[result bytes];
        NSMutableString *output = [NSMutableString stringWithCapacity:result.length * 2];
        for(int i = 0; i < result.length; i++){
            [output appendFormat:@"%02x", datas[i]];
        }
        return [output uppercaseString];
    }
    return nil;
}

-(NSString *)jh_aes256_decrypt:(NSString *)key
{
    //转换为2进制Data
    NSMutableData *data = [NSMutableData dataWithCapacity:self.length / 2];
    unsigned char whole_byte;
    char byte_chars[3] = {'\0','\0','\0'};
    int i;
    for (i=0; i < [self length] / 2; i++) {
        byte_chars[0] = [self characterAtIndex:i*2];
        byte_chars[1] = [self characterAtIndex:i*2+1];
        whole_byte = strtol(byte_chars, NULL, 16);
        [data appendBytes:&whole_byte length:1];
    }
    
    //对数据进行解密
    NSData* result = [data jh_aes256_decrypt:key];
    if (result && result.length > 0) {
        return [[NSString alloc] initWithData:result encoding:NSUTF8StringEncoding];
    }
    return nil;
}

@end



@implementation NSData (JHASE256)

- (NSData *)jh_aes256_encrypt:(NSString *)key
{
    char keyPtr[kCCKeySizeMaxRC4+1];
    bzero(keyPtr, sizeof(keyPtr));
    [key getCString:keyPtr maxLength:sizeof(keyPtr) encoding:NSUTF8StringEncoding];
    NSUInteger dataLength = [self length];
    size_t bufferSize = dataLength + kCCBlockSizeAES128;
    void *buffer = malloc(bufferSize);
    size_t numBytesEncrypted = 0;
    CCCryptorStatus cryptStatus = CCCrypt(kCCEncrypt, kCCAlgorithmAES128,
                                          kCCOptionPKCS7Padding ,
                                          keyPtr, kCCBlockSizeAES128,
                                          [AES_IV UTF8String],
                                          [self bytes], dataLength,
                                          buffer, bufferSize,
                                          &numBytesEncrypted);
    if (cryptStatus == kCCSuccess) {
        return [NSData dataWithBytesNoCopy:buffer length:numBytesEncrypted];
    }
    free(buffer);
    return nil;
}

- (NSData *)jh_aes256_decrypt:(NSString *)key
{
    char keyPtr[kCCKeySizeAES256+1];
    bzero(keyPtr, sizeof(keyPtr));
    [key getCString:keyPtr maxLength:sizeof(keyPtr) encoding:NSUTF8StringEncoding];
    NSUInteger dataLength = [self length];
    size_t bufferSize = dataLength + kCCBlockSizeAES128;
    void *buffer = malloc(bufferSize);
    size_t numBytesDecrypted = 0;
    CCCryptorStatus cryptStatus = CCCrypt(kCCDecrypt, kCCAlgorithmAES128,
                                          kCCOptionPKCS7Padding ,
                                          keyPtr, kCCBlockSizeAES128,
                                          [AES_IV UTF8String],
                                          [self bytes], dataLength,
                                          buffer, bufferSize,
                                          &numBytesDecrypted);
    if (cryptStatus == kCCSuccess) {
        return [NSData dataWithBytesNoCopy:buffer length:numBytesDecrypted];
        
    }
    free(buffer);
    return nil;
}

@end

```
