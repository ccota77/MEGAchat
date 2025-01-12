
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM (NSInteger, MEGAChatSessionStatus) {
    MEGAChatSessionStatusInvalid = 0xFF,
    MEGAChatSessionStatusInProgress = 0,
    MEGAChatSessionStatusDestroyed
};

typedef NS_ENUM (NSInteger, MEGAChatSessionChange) {
    MEGAChatSessionChangeNoChanges = 0x00,
    MEGAChatSessionChangeStatus = 0x01,
    MEGAChatSessionChangeRemoteAvFlags = 0x02,
    MEGAChatSessionChangeSpeakRequested = 0x04,
    MEGAChatSessionChangeOnLowRes = 0x08,
    MEGAChatSessionChangeOnHiRes = 0x10,
    MEGAChatSessionChangeOnHold = 0x20,
    MEGAChatSessionChangeAudioLevel = 0x40,
    MEGAChatSessionChangePermissions = 0x80,
    MEGAChatSessionChangeSpeakPermission = 0x100,
    MEGAChatSessionChangeOnRecording = 0x200,
};

typedef NS_ENUM (NSInteger, MEGAChatSessionTermCode) {
    MEGAChatSessionTermCodeInvalid = -1,
    MEGAChatSessionTermCodeRecoverable = 0,
    MEGAChatSessionTermCodeNonRecoverable = 1
};

@interface MEGAChatSession : NSObject

@property (nonatomic, readonly) MEGAChatSessionStatus status;
@property (nonatomic, readonly) MEGAChatSessionTermCode termCode;

@property (nonatomic, readonly, getter=hasAudio) BOOL audio;
@property (nonatomic, readonly, getter=hasVideo) BOOL video;

@property (nonatomic, readonly) uint64_t peerId;
@property (nonatomic, readonly) uint64_t clientId;
@property (nonatomic, readonly, getter=isOnHold) BOOL onHold;
@property (nonatomic, readonly) MEGAChatSessionChange changes;

@property (nonatomic, readonly, getter=isHighResVideo) BOOL highResVideo;
@property (nonatomic, readonly, getter=isLowResVideo) BOOL lowResVideo;
@property (nonatomic, readonly) BOOL canReceiveVideoHiRes;
@property (nonatomic, readonly) BOOL canReceiveVideoLowRes;

@property (nonatomic, readonly, getter=hasCamera) BOOL camera;
@property (nonatomic, readonly, getter=isLowResCamera) BOOL lowResCamera;
@property (nonatomic, readonly, getter=isHiResCamera) BOOL hiResCamera;

@property (nonatomic, readonly, getter=hasScreenShare) BOOL screenShare;
@property (nonatomic, readonly, getter=isLowResScreenShare) BOOL lowResScreenShare;
@property (nonatomic, readonly, getter=isHiResScreenShare) BOOL hiResScreenShare;

@property (nonatomic, readonly, getter=isModerator) BOOL moderator;
@property (nonatomic, readonly, getter=isAudioDetected) BOOL audioDetected;
@property (nonatomic, readonly, getter=isRecording) BOOL recording;

@property (nonatomic, readonly, getter=isSpeakAllowed) BOOL speakAllowed;
@property (nonatomic, readonly, getter=hasSpeakPermission) BOOL speakPermission;
@property (nonatomic, readonly, getter=hasPendingSpeakRequest) BOOL pendingSpeakRequest;

- (BOOL)hasChanged:(MEGAChatSessionChange)change;

@end

NS_ASSUME_NONNULL_END
