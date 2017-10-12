/**
    @file       BinInfo.h
    @brief      BIN FILE LAYOUT of f/w main code

    Copyright   Novatek Microelectronics Corp. 2012.  All rights reserved.
*/
#ifndef _BIN_INFO_H
#define _BIN_INFO_H

#define BININFO_FULLCOMPRESS_OFFSET         (0)         // full compressed flag (filled by bfc.exe)
#define BININFO_FULLCOMPRESS_CHKSUM_OFFSET  (0x04)      // checksum offset when full compressed (filled by bfc.exe)
#define BININFO_FULLCOMPRESS_LENGTH_OFFSET  (0x0C)      // length of compressed data (filled by bfc.exe)

#define BININFO_FW_LENGTH_OFFSET            (0x68)      // f/w file length (filled by encrypt_bin)
#define BININFO_CHKSUM_OFFSET               (0x6C)      // Checksum/RCR offset (filled by encrypt_bin)
#define BININFO_LENGTH_CHECK_OFFSET         (0x70)      // CRC checked length offset (filled by encrypt_bin)

#define BININFO_FW_COMPRESS_OFFSET          (0x78)      // f/w compressed flag (filled by bfc.exe)
#define FW_COMPRESS_MSK                     (1 << 0)    // bit 0: compressed enable

#define BININFO_BINARY_TAG_OFFSET           (0x7C)      // CRC binary tag offset (filled by encrypt_bin)

#define BININFO_LOADER_INFO_OFFSET          (0x80)      // loader info string (filled by loader)

#define BININFO_LDCTRL_OFFSET               (0x90)      // loader control word (filled by main code make)
#define LDCTRL_PARTIAL_LOAD_MSK             (1 << 0)    // bit 0: partial load enable

#define BININFO_LD_RUN_FLAG_OFFSET          (0x94)      // loader run-time flag for f/w (filled by loader)
#define BININFO_REAL_LOADLEN_OFFSET         (0x98)      // Real/Actual loaded length by loader (filled by loader)
#define BININFO_ELAPSETIME_OFFSET           (0x9C)      // loader elapse time (filled by loader)

//#NT#2012/12/17#HH Chuang -begin
//#NT#Change reserved block from block unit to byte unit
#define BININFO_LD_RESERVED_BLOCK_OFFSET    (0xA0)      // Reserved block offset (unit: byte) (filled by loader)
#define BININFO_RESERVED_SIZE_OFFSET        (0xA4)      // Reserved size (filled by loader)
//#NT#2012/12/17#HH Chuang -end

#define BININFO_FW_PART1_SIZE_OFFSET        (0x30C)     // Part1 size or non compressed part size (filled by main code make or bfc.exe)

#endif
