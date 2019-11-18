#include "GSavedGameList.h"
#include <BStore.h>
#include <Panic.h>
#include <dirent.h>
#include <sys/stat.h>

GSavedGameList gSavedGameList;

GSavedGameNode::GSavedGameNode(const char *aFilename, TInt32 aOrdinal) : BNodePri(aOrdinal) {
  mFilename = strdup(&aFilename[0]);
  sprintf(mDisplayName, "Saved Game #%d", pri);
  mOrdinal = aOrdinal;
}

GSavedGameNode::~GSavedGameNode() {
  delete[] mFilename;
}

void GSavedGameNode::Dump() {
  char formatted[4096];
//  ctime_r((time_t *)&mDate, formatted);
  sprintf(formatted, "Saved Game #%d", pri);
  printf("%s %s\n", mFilename, formatted);
}

GSavedGameList::GSavedGameList() {
  //
  mNumSavedGames = 0;
}
GSavedGameList::~GSavedGameList() {
  Reset();
}

void GSavedGameList::LoadSavedGameList() {
  Reset();
  mNumSavedGames = 0;
  mMaxGameNumber = 0;
  char name[4096];
  BStore store(SAVED_GAME_STORE);
  DIR *dir = opendir(store.mTargetDir);
  if (dir == ENull) {
    Panic("Can't read saved games directory (%s)\n", store.mTargetDir);
  }
  while (struct dirent *d = readdir(dir)) {
    if (strstr(d->d_name, SAVED_GAME_STORE) != ENull) {
      struct stat statbuf;
      sprintf(name, "%s/%s", store.mTargetDir, d->d_name);
      stat(name, &statbuf);
      TInt32 game_number = 0;
      char *ptr = strchr(d->d_name, '0');
      if (!ptr) {
        continue;
      }
      while (isdigit(*ptr)) {
        game_number *= 10;
        game_number += *ptr++ - '0';
      }
      if (game_number > mMaxGameNumber) {
        mMaxGameNumber = game_number;
      }
      ++mNumSavedGames;
      auto *n = new GSavedGameNode(d->d_name, TInt32(game_number));
      Add(*n);
    }
  }
}

void GSavedGameList::SaveGame(TUint8 *aData, TUint32 aSize, char *aSavedName) {
  char game_name[128];
  TInt game_number = mMaxGameNumber + 1;
  LoadSavedGameList();
  GSavedGameNode::GameName(game_name, game_number);
  BStore store(SAVED_GAME_STORE);
  store.Set(game_name, aData, aSize);
  if (aSavedName != ENull) {
    sprintf(aSavedName, "Saved Game #%d", game_number);
  }
  LoadSavedGameList();
}

BMemoryStream *GSavedGameList::LoadSavedGame(const char *aName) {
  BStore store(SAVED_GAME_STORE);
  TUint32 size = store.Size(aName);
  TUint8 data[size];
  store.Get(aName, data, size);

  return new BMemoryStream(data, size);
}

BMemoryStream *GSavedGameList::LoadSavedGame(GSavedGameNode *aNode) {
  char name[256];
  aNode->GameName(name);
  return LoadSavedGame(name);
}

void GSavedGameList::RemoveGame(const char *aGameName) {
  BStore store(SAVED_GAME_STORE);
  store.Remove(aGameName);
}



void GSavedGameList::RemoveGame(GSavedGameNode *aGameNode){
  char game_name[128];
  LoadSavedGameList();
  GSavedGameNode::GameName(game_name, aGameNode->mOrdinal);

  BStore store(SAVED_GAME_STORE);
  store.Remove(game_name);
  LoadSavedGameList();
}

void GSavedGameList::Dump() {
  for (GSavedGameNode *n = First(); !End(n); n = Next(n)) {
    n->Dump();
  }
}


