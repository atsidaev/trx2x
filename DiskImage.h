#ifndef __DISKIMAGE_H
#define __DISKIMAGE_H

#include <stdint.h>

//-----------------------------------------------------------------------------

enum TDiskImageType { DIT_UNK, DIT_TRD, DIT_SCL, DIT_FDI, DIT_TD0, DIT_UDI, DIT_HOB, DIT_FDD };

struct VGFIND_TRACK
{
   uint8_t *TrackPointer;
   uint8_t *ClkPointer;
   uint32_t TrackLength;
   uint8_t FoundTrack;
};


struct VGFIND_ADM
{
   uint8_t* TrackPointer;
   uint8_t* ClkPointer;
   uint32_t TrackLength;

   uint8_t *ADMPointer;
   uint32_t ADMLength;

   uint32_t MarkedOffsetADM;
   uint32_t OffsetADM;
   uint32_t OffsetEndADM;
   uint8_t FoundADM;
   uint8_t CRCOK;
};


struct VGFIND_SECTOR
{
   VGFIND_ADM vgfa;

   uint8_t *SectorPointer;
   uint32_t SectorLength;

   uint32_t MarkedOffsetSector;
   uint32_t OffsetSector;
   uint32_t OffsetEndSector;
   uint8_t FoundDATA;
   uint8_t CRCOK;
   uint8_t DataMarker;
};


class TDiskImage
{
   uint32_t FTrackLength[256][256];
   uint8_t* FTracksPtr[256][256][2];

   char FFileName[4096];
   TDiskImageType FType;
   uint8_t Fdefbuf[16384];

   uint16_t MakeVGCRC(unsigned char *data, unsigned long length);
public:
   bool Changed;

   bool ReadOnly;
   bool DiskPresent;
   uint8_t MaxTrack;
   uint8_t MaxSide;

   bool AddBOOT;

   TDiskImage();
   ~TDiskImage();

   bool FindTrack(uint8_t CYL, uint8_t SIDE, VGFIND_TRACK *vgft);
   bool FindADMark(uint8_t CYL, uint8_t SIDE,
                   uint32_t FromOffset, 
                   VGFIND_ADM *vgfa);
   bool FindSector(uint8_t CYL, uint8_t SIDE,
                   uint8_t SECT, 
                   VGFIND_SECTOR *vgfs, uint32_t FromOffset=0);
   void ApplySectorCRC(VGFIND_SECTOR vgfs);


   void FlushImage();
   void Open(char *filename, bool ReadOnly);
   char* GetDiskFileName() {return (char*)FFileName; }

   void readTRD(int hfile, bool readonly);
   void writeTRD(int hfile);
   void readSCL(int hfile, bool readonly);
   void writeSCL(int hfile);
   void readFDI(int hfile, bool readonly);
   void writeFDI(int hfile);
   void readUDI(int hfile, bool readonly);
   void writeUDI(int hfile);
   void readTD0(int hfile, bool readonly);
   void writeTD0(int hfile);
   void readFDD(int hfile, bool readonly);
   void writeFDD(int hfile);
   void readHOB(int hfile);

   void formatTRDOS(uint32_t tracks, uint32_t sides);

   void ShowError(const char *str);
};

#pragma pack(1)
struct UDI_HEADER               // 16 bytes
{
   uint8_t ID[4];
   uint32_t UnpackedLength;
   uint8_t Version;
   uint8_t MaxCylinder;
   uint8_t MaxSide;
   uint8_t _zero;
   uint32_t ExtHdrLength;
};

struct TD0_MAIN_HEADER          // 12 bytes
{
   char ID[2];                  // +0:  "TD" - 'Normal'; "td" - packed LZH ('New Advanced data compression')
   uint8_t __t;           // +2:  = 0x00
   uint8_t __1;           // +3:  ???
   uint8_t Ver;           // +4:  Source version  (1.0 -> 10, ..., 2.1 -> 21)
   uint8_t __2;           // +5:  ???
   uint8_t DiskType;      // +6:  Source disk type
   uint8_t Info;          // +7:  D7-наличие image info
   uint8_t DataDOS;       // +8:  if(=0)'All sectors were copied', else'DOS Allocated sectors were copied'
   uint8_t ChkdSides;     // +9:  if(=1)'One side was checked', else'Both sides were checked'
   uint16_t CRC;          // +A:  CRC хидера TD0_MAIN_HEADER (кроме байт с CRC)
};

struct TD0_INFO_DATA             // 10 байт без строки коментария...
{
   uint16_t CRC;          // +0:  CRC для структуры COMMENT_DATA (без байтов CRC)
   uint16_t strLen;       // +2:  Длина строки коментария 
   uint8_t Year;          // +4:  Дата создания - год (1900 + X)
   uint8_t Month;         // +5:  Дата создания - месяц (Январь=0, Февраль=1,...)
   uint8_t Day;           // +6:  Дата создания - число
   uint8_t Hours;         // +7:  Время создания - часы
   uint8_t Minutes;       // +8:  Время создания - минуты
   uint8_t Seconds;       // +9:  Время создания - секунды
};

struct TD0_TRACK_HEADER         // 4 bytes
{
   uint8_t SectorCount;
   uint8_t Track;
   uint8_t Side;
   uint8_t CRCL;
};

struct TD0_SECT_HEADER          // 8 bytes
{
   uint8_t ADRM[6];
   uint16_t DataLength;
};

struct FDD_MAIN_HEADER 
{
    char ID[30];                /* сигнатура */
    uint8_t MaxTracks;    /* число треков (цилиндров) */
    uint8_t MaxHeads;     /* число головок (1 или 2) */
    uint32_t diskIndex;             /* unused */
    uint32_t DataOffset[512*2];     /* смещение в файле к структурам заголовков */
                                /* треков       */
};

struct FDD_TRACK_HEADER
{
    uint8_t trkType;      /* unused */
    uint8_t SectNum;      /* число секторов на треке */
    struct
    {
        /* заголовок сектора */
         uint8_t trk;     /* номер трека */
         uint8_t side;    /* номер стороны */
                                /* 7 бит этого байта указывает бит a */
         uint8_t sect;    /* номер сектора */
         uint8_t size;    /* размер сектора (код) */
         uint32_t SectPos;          /* смещение в файле к данным сектора */
    } sect[256];
};


struct TRDOS_DIR_ELEMENT        // 16 bytes
{
   char FileName[8];
   char Type;
   uint16_t Start;
   uint16_t Length;
   uint8_t SecLen;
   uint8_t FirstSec;
   uint8_t FirstTrk;
};
#pragma pack()

//-----------------------------------------------------------------------------
#endif
