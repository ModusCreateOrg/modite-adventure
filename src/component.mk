.PHONY: Resources.bin
	echo "THERE"
	cd ${CREATIVE_ENGINE_PATH}/tools && make
	${CREATIVE_ENGINE_PATH}/tools/rcomp Resources.r

COMPONENT_NAME = Evade

COMPONENT_EMBED_FILES := Resources.bin

COMPONENT_ADD_INCLUDEDIRS = . \
  SplashState \
  TitleState \
  GameState \

COMPONENT_SRCDIRS = ${COMPONENT_ADD_INCLUDEDIRS}
