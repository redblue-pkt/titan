#!/bin/sh
echo "[reset.sh] start"

. /sbin/start-function

startUserRestore $1
startUserSettings

echo "[reset.sh] end"