#ifndef GSAVEDGAMELIST_H
#define GSAVEDGAMELIST_H

#include <BTypes.h>
#include <BList.h>

static const char *SAVED_GAME_STORE = "saved_game";

class GSavedGameNode : public BNodePri {
public:
  char *mFilename;
  TInt32 mDate;

public:
  GSavedGameNode(const char *aFilename, TInt32 aDate);
  ~GSavedGameNode();

public:
  void Dump();
};

class GSavedGameList : public BListPri {
public:
  GSavedGameList();
  ~GSavedGameList();

public:
  TInt mNumSavedGames;
  //
  void LoadSavedGameList();

  void Dump();

public:
  GSavedGameNode *First() {
    return (GSavedGameNode *)BListPri::First();
  }

  TBool End(GSavedGameNode *game) {
    return BListPri::End((BNodePri *)game);
  }

  GSavedGameNode *Next(GSavedGameNode *game) {
    return (GSavedGameNode *)BListPri::Next((BNodePri *)game);
  }
};

extern GSavedGameList gSavedGameList;

#endif
