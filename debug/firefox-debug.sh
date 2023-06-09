#!/bin/bash

export USEARCH_DEBUG=1
export MOZ_FUZZ_DEBUG=1
export USE_DEBUGGER=1
export DEBUG=1

export MOZ_LOG=timestamp,rotate:200,nsHttp:5,cache2:5,nsSocketTransport:5,nsHostResolver:5
export MOZ_LOG_FILE=/tmp/log.txt

