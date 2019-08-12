.PHONY: Resources.bin
	cd ${CREATIVE_ENGINE_PATH}/tools/rcomp && make
	${CREATIVE_ENGINE_PATH}/tools/rcomp Resources.r

COMPONENT_NAME = Evade

COMPONENT_EMBED_FILES := Resources.bin

COMPONENT_ADD_INCLUDEDIRS = . \
  SplashState \
  GameState \
  GameMenuState \
  MainMenuState \
  MainOptionsState \
  CreditsState \

COMPONENT_SRCDIRS = ${COMPONENT_ADD_INCLUDEDIRS}
