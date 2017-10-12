#ifndef MOVIE_MAPPING_H
#define MOVIE_MAPPING_H

extern UINT32   GetMovieSizeWidth(UINT32 uiIndex);
extern UINT32   GetMovieSizeHeight(UINT32 uiIndex);
extern UINT32   GetMovieFrameRate(UINT32 uiIndex);
//extern UINT32   GetMovieQualityValue(UINT32 uiIndex);
extern UINT32   GetMovieTargetBitrate(UINT32 uiSize, UINT32 uiQuality);
extern void     SetMovieTargetBitrate(UINT32 uiType, UINT32 uiSize, UINT32 uiBitRate);
extern UINT32   GetMovieDispAspectRatio(UINT32 uiIndex);
extern UINT32   GetMovieAudioVolumeValue(UINT32 uiIndex);
extern UINT32   Get_MovieFormatValue(UINT32 uhIndex);


extern UINT32   GetMovieSizeWidth_2p(UINT32 uiPath, UINT32 uiIndex);
extern UINT32   GetMovieSizeHeight_2p(UINT32 uiPath, UINT32 uiIndex);
extern UINT32   GetMovieFrameRate_2p(UINT32 uiPath, UINT32 uiIndex);
extern UINT32   GetMovieTargetBitrate_2p(UINT32 uiPath, UINT32 uiIndex);
extern UINT32   GetMovieDispAspectRatio_2p(UINT32 uiPath, UINT32 uiIndex);

#endif
