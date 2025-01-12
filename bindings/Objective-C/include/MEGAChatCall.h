
#import <Foundation/Foundation.h>
#import "MEGAHandleList.h"
#import "MEGAChatSession.h"
#import "MEGAChatWaitingRoom.h"

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM (NSInteger, MEGAChatCallStatus) {
    MEGAChatCallStatusUndefined = -1,
    MEGAChatCallStatusInitial = 0,
    MEGAChatCallStatusUserNoPresent,
    MEGAChatCallStatusConnecting,
    MEGAChatCallStatusWaitingRoom,
    MEGAChatCallStatusJoining,
    MEGAChatCallStatusInProgress,
    MEGAChatCallStatusTerminatingUserParticipation,
    MEGAChatCallStatusDestroyed
};

typedef NS_ENUM (NSInteger, MEGAChatCallSessionStatus) {
    MEGAChatCallSessionStatusInitial = 0,
    MEGAChatCallSessionStatusInProgress,
    MEGAChatCallSessionStatusDestroyed,
    MEGAChatCallSessionStatusNoSession
};

typedef NS_ENUM (NSInteger, MEGAChatCallTermCode) {
    MEGAChatCallTermCodeInvalid = -1,
    MEGAChatCallTermCodeUserHangup = 0,
    MEGAChatCallTermCodeTooManyParticipants = 1,
    MEGAChatCallTermCodeCallReject = 2,
    MEGAChatCallTermCodeError = 3,
    MEGAChatCallTermCodeNoParticipate = 4,
    MEGAChatCallTermCodeTooManyClients = 5,
    MEGAChatCallTermCodeProtocolVersion = 6,
    MEGAChatCallTermCodeKicked = 7,
    MEGAChatCallTermCodeWaitingRoomTimeout = 8,
};

typedef NS_ENUM (NSInteger, MEGAChatCallChangeType) {
    MEGAChatCallChangeTypeNoChanges = 0x00,
    MEGAChatCallChangeTypeStatus = 0x01,
    MEGAChatCallChangeTypeLocalAVFlags = 0x02,
    MEGAChatCallChangeTypeRingingStatus = 0x04,
    MEGAChatCallChangeTypeCallComposition = 0x08,
    MEGAChatCallChangeTypeCallOnHold = 0x10,
    MEGAChatCallChangeTypeCallSpeak = 0x20,
    MEGAChatCallChangeTypeAudioLevel = 0x40,
    MEGAChatCallChangeTypeNetworkQuality = 0x80,
    MEGAChatCallChangeTypeOutgoingRingingStop = 0x100,
    MEGAChatCallChangeTypeOwnPermissions = 0x200,
    MEGAChatCallChangeTypeGenericNotification = 0x400,
    MEGAChatCallChangeTypeWaitingRoomAllow = 0x800,
    MEGAChatCallChangeTypeWaitingRoomDeny = 0x1000,
    MEGAChatCallChangeTypeWaitingRoomComposition = 0x2000,
    MEGAChatCallChangeTypeWaitingRoomUsersEntered = 0x4000,
    MEGAChatCallChangeTypeWaitingRoomUsersLeave = 0x8000,
    MEGAChatCallChangeTypeWaitingRoomUsersAllow = 0x10000,
    MEGAChatCallChangeTypeWaitingRoomUsersDeny = 0x20000,
    MEGAChatCallChangeTypeWaitingRoomPushedFromCall = 0x40000,
};

typedef NS_ENUM (NSInteger, MEGAChatCallConfiguration) {
    MEGAChatCallConfigurationWithAudio = 0,
    MEGAChatCallConfigurationWithVideo = 1,
    MEGAChatCallConfigurationAnyFlag = 2,
};

typedef NS_ENUM (NSInteger, MEGAChatCallCompositionChange) {
    MEGAChatCallCompositionChangePeerRemoved = -1,
    MEGAChatCallCompositionChangeNoChange = 0,
    MEGAChatCallCompositionChangePeerAdded = 1,
};

typedef NS_ENUM (NSInteger, MEGAChatCallNetworkQuality) {
    MEGAChatCallNetworkQualityBad = 0,
    MEGAChatCallNetworkQualityGood = 1,
};

typedef NS_ENUM (NSInteger, MEGAChatCallNotificationType) {
    MEGAChatCallNotificationTypeInvalid = 0,
    MEGAChatCallNotificationTypeSfuError = 1,
    MEGAChatCallNotificationTypeSfuDeny = 2,
};

@interface MEGAChatCall : NSObject

@property (nonatomic, readonly) MEGAChatCallStatus status;
@property (nonatomic, readonly) uint64_t chatId;
@property (nonatomic, readonly) uint64_t callId;
@property (nonatomic, readonly) MEGAChatCallChangeType changes;
@property (nonatomic, readonly) int64_t duration;
@property (nonatomic, readonly) int64_t initialTimeStamp;
@property (nonatomic, readonly) int64_t finalTimeStamp;
@property (nonatomic, readonly, getter=hasLocalAudio) BOOL localAudio;
@property (nonatomic, readonly, getter=hasLocalVideo) BOOL localVideo;
@property (nonatomic, readonly) MEGAChatCallTermCode termCode;
@property (nonatomic, readonly, getter=isRinging) BOOL ringing;
@property (nonatomic, readonly) uint64_t peeridCallCompositionChange;
@property (nonatomic, readonly) MEGAChatCallCompositionChange callCompositionChange;

@property (nonatomic, readonly) NSInteger numParticipants;
@property (nonatomic, readonly, getter=isOnHold) BOOL onHold;
@property (nonatomic, readonly) MEGAChatCallNetworkQuality networkQuality;
@property (nonatomic, readonly) MEGAHandleList *sessionsClientId;

@property (nonatomic, readonly) MEGAHandleList *participants;

@property (nonatomic, readonly) NSUUID *uuid;

@property (nonatomic, readonly) MEGAChatWaitingRoomStatus waitingRoomJoiningStatus;
@property (nonatomic, readonly) MEGAChatWaitingRoom *waitingRoom;
@property (nonatomic, readonly) MEGAHandleList *waitingRoomHandleList;

- (BOOL)hasChangedForType:(MEGAChatCallChangeType)changeType;

- (nullable MEGAChatSession *)sessionForClientId:(uint64_t)clientId;

- (instancetype)clone;

- (NSInteger)notificationType;

- (NSString *)termcodeString:(MEGAChatCallTermCode)termcode;

- (NSString *)genericMessage;

+ (NSString *)stringForTermCode:(MEGAChatCallTermCode)termCode;

@end

NS_ASSUME_NONNULL_END
