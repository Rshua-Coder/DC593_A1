#ifndef _JPG2MOVIE_H
#define _JPG2MOVIE_H

typedef void (*JPG2MOV_PROGRESS_CB)(UINT32 uiProgress);

typedef struct
{
    JPG2MOV_PROGRESS_CB     fpProgressCB;

} JPG2MOV_OBJ;

extern INT32 JPG2MOV_MakeMovie(JPG2MOV_OBJ *pObj);

#endif
