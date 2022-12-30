
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM (NSInteger, MEGAChatScheduledFlagsType) {
    MEGAChatScheduledFlagsTypeDontSendEmails = 0,
};

@interface MEGAChatScheduledFlags : NSObject

- (instancetype)clone;

@property (readonly, nonatomic) BOOL emailsDisabled;
@property (readonly, nonatomic) BOOL isEmpty;

- (void)setEmailsDisabled:(BOOL)disable;
- (void)reset;

@end

NS_ASSUME_NONNULL_END