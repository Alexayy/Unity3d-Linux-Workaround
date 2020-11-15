#!/bin/bash

PREFIX=.mount_Unity

if [ -z "$TMPDIR" ]
then
  echo TMPDIR not set, taking /tmp
  export TMPDIR=/tmp
fi

FAKE_PREFIX=$TMPDIR/$PREFIX"_tuxad"
cd $(dirname $0) || exit 111
SCRIPT_DIR=$(pwd)

if [ "$1" ]
then
  UHS_EXECUTABLE_PATH=$1
  if [ -e "$UHS_EXECUTABLE_PATH" ]
  then
    echo Using UnityHubSetup executable: $UHS_EXECUTABLE_PATH
  else
    echo 'ERROR: Command line specified "$UHS_EXECUTABLE_PATH" does not exist. Exiting...'
    exit 111
  fi
else
  UHS_EXECUTABLE=$(ls|grep -i "unityhub.*setup")
  if [ "$UHS_EXECUTABLE" ]
  then
    UHS_EXECUTABLE_PATH=$SCRIPT_DIR/$UHS_EXECUTABLE
    echo Found UnityHubSetup executable: $UHS_EXECUTABLE_PATH
  else
    echo 'ERROR: Could not found UnityHubSetup executable. Please copy it to script dir or specify it (incl. path) as command line argument. Exiting...'
    exit 111
  fi
fi

kill_pids() {
  ps axwf|grep -v grep|fgrep '/unityhub --'|awk '{print$1}'|tac|while read pid;do kill -TERM $pid;sleep .2;done
  ps axwf|grep -v grep|fgrep '/unityhub --'|awk '{print$1}'|tac|while read pid;do kill -9 $pid;sleep .2;done
  ps axwf|grep -v grep|fgrep '/unityhub'|awk '{print$1}'|tac|while read pid;do kill -TERM $pid;sleep .2;done
  ps axwf|grep -v grep|fgrep '/unityhub'|awk '{print$1}'|tac|while read pid;do kill -9 $pid;sleep .2;done
}

echo Killing any currently running UnityHubSetups
kill_pids

echo Deleting existing image dirs $TMPDIR/$PREFIX'*'
/bin/rm -rf $TMPDIR/$PREFIX*

if ls -ld $TMPDIR/$PREFIX* &>/dev/null
then
  echo 'ERROR: Could not delete all directories, insufficient permissions? Exiting...'
  exit 1
fi

mkdir $FAKE_PREFIX

echo Starting $UHS_EXECUTABLE_PATH in background
( export DISPLAY=:0; "$UHS_EXECUTABLE_PATH"; ) &
echo Waiting for unityhub image dir to appear...

cat <<EOF
================================================
==                                            ==
==  Please wait for final start of installer  ==
==                                            ==
================================================
EOF

while sleep 0.2
do
  UNITY_HUB=$(ls -d $TMPDIR/$PREFIX*/unityhub 2>/dev/null)
  [ "$UNITY_HUB" ] && break
done
UNITYHUB_DIR=$(echo $UNITY_HUB |sed 's,\/unityhub$,,')

# just wait a short time to be on safe side
sleep 2

echo Found: $UNITY_HUB
echo Copying $UNITYHUB_DIR to $FAKE_PREFIX
if /bin/cp -af $UNITYHUB_DIR/. $FAKE_PREFIX
then
  echo Copied successfully
else
  echo ERROR: cp did not finish successfully, exiting.
  exit 2
fi

echo Killing UnitySetupSetup
kill_pids

if test -e $FAKE_PREFIX/unityhub.bin
then
  echo $FAKE_PREFIX/unityhub.bin 'already exists, previously started fake detected?'
else
  if /bin/cp -af $FAKE_PREFIX/unityhub $FAKE_PREFIX/unityhub.bin
  then
    echo Original unityhub executable copied to unityhub.bin
  else
    echo 'ERROR: could not copy unityhub executable, exiting.'
    exit 3
  fi
fi

echo Installing fake wrapper script
echo '#!/bin/bash
export LD_PRELOAD=wrap_statfs.so
export LD_LIBRARY_PATH='$FAKE_PREFIX'/usr/lib
exec "$0".bin $@' >$FAKE_PREFIX/unityhub

echo Copying fake library for preloading
if /bin/cp -af wrap_statfs.so $FAKE_PREFIX/usr/lib/
then
  echo Fake library installed in $FAKE_PREFIX/usr/lib/wrap_statfs.so
else
  echo 'ERROR: could not copy fake library to '$FAKE_PREFIX/usr/lib/wrap_statfs.so', exiting'
  exit 4
fi

echo Launching faked installer
cat <<EOF
===================================================
==                                               ==
==  Final launch of "diskspace faked installer"  ==
==                                               ==
===================================================
EOF
exec $FAKE_PREFIX/unityhub

