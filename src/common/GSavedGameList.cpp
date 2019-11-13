#include "GSavedGameList.h"
#include <BStore.h>
#include <Panic.h>
#include <dirent.h>
#include <sys/stat.h>

GSavedGameList gSavedGameList;

GSavedGameNode::GSavedGameNode(const char *aFilename, TInt32 aDate) : BNodePri(aDate) {
  mFilename = strdup(aFilename);
  mDate = aDate;
}

GSavedGameNode::~GSavedGameNode() {
  delete[] mFilename;
}

void GSavedGameNode::Dump() {
  char formatted[4096];
  ctime_r((time_t *)&mDate, formatted);
  printf("%s %s\n", mFilename, formatted);
}

GSavedGameList::GSavedGameList() {
  //
}
GSavedGameList::~GSavedGameList() {
  Reset();
}

void GSavedGameList::LoadSavedGameList() {
  Reset();
  mNumSavedGames = 0;
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
      GSavedGameNode *n = new GSavedGameNode(d->d_name, TInt32(statbuf.st_mtime));
      Add(*n);
      mNumSavedGames++;
    }
  }
}

void GSavedGameList::Dump() {
  for (GSavedGameNode *n = First(); !End(n); n = Next(n)) {
    n->Dump();
  }
}
