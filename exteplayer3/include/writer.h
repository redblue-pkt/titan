#ifndef WRITER_H_
#define WRITER_H_

#include <sys/uio.h>
#include <stdio.h>
#include <stdint.h>

typedef enum { eNone, eAudio, eVideo, eGfx} eWriterType_t;

typedef struct {
    int                    fd;
    unsigned char*         data;
    unsigned int           len;
    unsigned long long int Pts;
    unsigned long long int Dts;
    unsigned char*         private_data;
    unsigned int           private_size;
    unsigned int           FrameRate;
    unsigned int           FrameScale;
    unsigned int           Width;
    unsigned int           Height;
    unsigned char          Version;
} WriterAVCallData_t;

typedef struct {
    unsigned char*         data;
    unsigned int           Width;
    unsigned int           Height;
    unsigned int           Stride;
    unsigned int           color;

    unsigned int           x;       /* dst x ->given by ass */
    unsigned int           y;       /* dst y ->given by ass */

    /* destination values if we use a shared framebuffer */
    int                    fd;
    unsigned int           Screen_Width;
    unsigned int           Screen_Height;
    uint32_t		   *destination;
    unsigned int           destStride;
} WriterFBCallData_t;

typedef struct WriterCaps_s {
    char*          name;
    eWriterType_t  type;
    char*          textEncoding;
    /* fixme: revise if this is an enum! */
    int            dvbEncoding;   /* For sh4    */
    int            dvbStreamType; /* For mipsel */
    int            dvbCodecType;  /* For mipsel */
} WriterCaps_t;

typedef struct Writer_s {
    int           (* reset) ();
    int           (* writeData) (void*);
    int           (* writeReverseData) (void*);
    WriterCaps_t *caps;
} Writer_t;

extern Writer_t WriterAudioLPCM;
extern Writer_t WriterAudioIPCM;
extern Writer_t WriterAudioPCM;
extern Writer_t WriterAudioMP3;
extern Writer_t WriterAudioMPEGL3;
extern Writer_t WriterAudioAC3;
extern Writer_t WriterAudioEAC3;
extern Writer_t WriterAudioAAC;
extern Writer_t WriterAudioAACHE;
extern Writer_t WriterAudioAACPLUS;
extern Writer_t WriterAudioDTS;
extern Writer_t WriterAudioWMA;
extern Writer_t WriterAudioWMAPRO;
extern Writer_t WriterAudioFLAC;
extern Writer_t WriterAudioVORBIS;
extern Writer_t WriterAudioAMR;

extern Writer_t WriterVideoMPEG1;
extern Writer_t WriterVideoMPEG2;
extern Writer_t WriterVideoMPEG4;
extern Writer_t WriterVideoMPEGH264;
extern Writer_t WriterVideoH264;
extern Writer_t WriterVideoDIVX3;
extern Writer_t WriterVideoWMV;
extern Writer_t WriterVideoDIVX;
extern Writer_t WriterVideoFOURCC;
extern Writer_t WriterVideoMSCOMP;
extern Writer_t WriterVideoH263;
extern Writer_t WriterVideoFLV;
extern Writer_t WriterVideoVC1;
extern Writer_t WriterFramebuffer;
extern Writer_t WriterPipe;
extern Writer_t WriterDVBSubtitle;

Writer_t* getWriter(char* encoding);

Writer_t* getDefaultVideoWriter();
Writer_t* getDefaultAudioWriter();
Writer_t* getDefaultFramebufferWriter();
ssize_t write_with_retry(int fd, const void *buf, size_t size);
ssize_t writev_with_retry(int fd, const struct iovec *iov, size_t ic);
#endif
