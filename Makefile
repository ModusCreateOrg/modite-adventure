## Generic makefile for simple utilities

ROOT_DIR := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
CREATIVE_ENGINE_PATH=$(ROOT_DIR)/creative-engine


release: FORCE
	./scripts/build.sh docker-build && cp ./build/ModiteAdventure.tgz ~/Downloads/

rcomp: FORCE
	echo "Building rcomp"
	cd ${CREATIVE_ENGINE_PATH}/tools/rcomp-src && make
	rm ${CREATIVE_ENGINE_PATH}/tools/rcomp-src/rcomp

resources: rcomp FORCE
	echo "Compiling resources"
	cd src && ${CREATIVE_ENGINE_PATH}/tools/rcomp Resources.r

reset: FORCE
	echo "Resetting high score table (and options)"
	rm -f cmake-build-debug/ModiteAdventure.app/Contents/MacOS/*.store

FORCE:
