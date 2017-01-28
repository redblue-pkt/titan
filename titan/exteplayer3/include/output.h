#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <stdio.h>

typedef enum {
    OUTPUT_INIT,
    OUTPUT_ADD,
    OUTPUT_DEL,
    OUTPUT_CAPABILITIES,
    OUTPUT_PLAY,
    OUTPUT_STOP,
    OUTPUT_PAUSE,
    OUTPUT_OPEN,
    OUTPUT_CLOSE,
    OUTPUT_FLUSH,
    OUTPUT_CONTINUE,
    OUTPUT_FASTFORWARD,
    OUTPUT_AVSYNC,
    OUTPUT_CLEAR,
    OUTPUT_PTS,
    OUTPUT_SWITCH,
    OUTPUT_SLOWMOTION,
    OUTPUT_AUDIOMUTE,
    OUTPUT_REVERSE,
    OUTPUT_DISCONTINUITY_REVERSE,
    OUTPUT_GET_FRAME_COUNT,
    OUTPUT_GET_PROGRESSIVE,
    OUTPUT_SUBTITLE_REGISTER_FUNCTION = 222,
    OUTPUT_SUBTITLE_REGISTER_BUFFER = 223,
    OUTPUT_GET_SUBTITLE_OUTPUT,
    OUTPUT_SET_SUBTITLE_OUTPUT
} OutputCmd_t;

typedef struct
{
    uint8_t         *data;
    uint32_t         len;

    uint8_t         *extradata;
    uint32_t         extralen;
    
    int64_t          pts;
    int64_t          dts;
     
    uint32_t         frameRate;
    uint32_t         timeScale;
    
    uint32_t         width;
    uint32_t         height;
    
    char            *type;
} AudioVideoOut_t;
/*
typedef struct
{
    uint32_t         trackId;
    uint8_t         *data;
    uint32_t         len;
    
    int64_t          pts;
    int64_t          durationMS; // duration in miliseconds
    
    char            *type;
} SubtitleOut_t;
*/

typedef struct Output_s 
{
    char * Name;
    int32_t (* Command) (/*Context_t*/void  *, OutputCmd_t, void *);
    int32_t (* Write) (/*Context_t*/void  *, void* privateData);
    char ** Capabilities;

} Output_t;

extern Output_t LinuxDvbOutput;
extern Output_t SubtitleOutput;
extern Output_t PipeOutput;

typedef struct OutputHandler_s 
{
    char *Name;
    Output_t *audio;
    Output_t *video;
    Output_t *subtitle;
    Output_t *dvbsubtitle;
    Output_t *teletext;
    int32_t (* Command) (/*Context_t*/void  *, OutputCmd_t, void *);
} OutputHandler_t;

#endif
