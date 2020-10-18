#ifndef GSAVEDGAMELIST_H
#define GSAVEDGAMELIST_H

#include <BTypes.h>
#include <BList.h>
#include <BMemoryStream.h>

static const char *SAVED_GAME_STORE = "ModiteAdventure";

class GSavedGameNode : public BNodePri {
public:
  char *mFilename;
  char mDisplayName[128];
  TInt32 mOrdinal;

public:
  GSavedGameNode(const char *aFilename, TInt32 aOrdinal);
  ~GSavedGameNode() OVERRIDE;

   static void GameName(char *aOut, TUint32 aOrdinal) {
    sprintf(aOut, "SavedGame%07d", aOrdinal);
  }
   void GameName(char *aOut) {
    sprintf(aOut, "SavedGame%07d", mOrdinal);
  }

public:
  void Dump();
};

class GSavedGameList : public BListPri {
public:
  GSavedGameList();
  ~GSavedGameList() OVERRIDE;

public:
  TInt mNumSavedGames;
  TInt mMaxGameNumber;
  //
  void LoadSavedGameList();

public:
  void SaveGame(TUint8 *aData, TUint32 aSize, char *aSavedName = ENull);
  BMemoryStream *LoadSavedGame(const char *aName);
  BMemoryStream *LoadSavedGame(GSavedGameNode *aNode);
  void RemoveGame(const char *aGameName);
  void RemoveGame(GSavedGameNode *aGameNode);
  void RemoveAllGames();

  void Dump();

public:
  GSavedGameNode *First() OVERRIDE {
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
