#!/bin/bash
# A simple testframe runner for the bzip2 test suite.
#
# Copyright (C) 2019, Mark J. Wielaard  <mark@klomp.org>
#
# This file is free software.  You can redistribute it and/or modify
# it under the terms of the GNU General Public License (GPL); either
# version 3, or (at your option) any later version.

VALGRIND="valgrind"
VALGRIND_ARGS="-q --error-exitcode=9"
BZIP2="bzip2"
TESTS_DIR="."
IGNORE_MD5=0

for i in "$@"
do
case $i in
    --bzip2=*)
    BZIP2="${i#*=}"
    shift
    ;;
    --without-valgrind)
    VALGRIND=""
    VALGRIND_ARGS=""
    shift
    ;;
    --ignore-md5)
    IGNORE_MD5=1
    shift
    ;;
    --tests-dir=*)
    TESTS_DIR="${i#*=}"
    ;;
    *)
    echo "run-tests [--bzip2=bzip2-command] [--without-valgrind]"
    echo "          [--ignore-md5] [--tests-dir=/path/to/bzip2-tests-dir]"
    exit 1
    ;;
esac
done

if ! type "valgrind" > /dev/null; then
  VALGRIND=""
  VALGRIND_ARGS=""
fi

echo "Testing ${BZIP2} in directory ${TESTS_DIR}"
if [ "$VALGRIND" != "" ]; then
  echo "  using valgrind"
else
  echo "  NOT using valgrind"
fi
if [[ ${IGNORE_MD5} -eq 0 ]]; then
  echo "  checking md5 sums"
else
  echo "  NOT checking md5 sums"
fi

# Remove any left over tesfilecopies from previous runs first.
find ${TESTS_DIR} -type f -name \*\.testfilecopy\.bz2 -exec rm \{\} \;

# First test the good bz2 files.
echo "Testing decompression and recompression..."
echo

nogood=0
while IFS= read -r -d '' bzfile; do
  file="${bzfile%.*}"
  copy="${file}.testfilecopy"
  bzcopy="${copy}.bz2"
  md5file="${file}.md5"

  echo "Processing ${bzfile}"

  # Decompress it.
  echo "  Decompress..."
  rm -f "${file}"
  ${VALGRIND} ${VALGRIND_ARGS} ${BZIP2} -k -d -q ${bzfile} \
    || { echo "!!! bad decompress result $?";
         nogood=$[${nogood}+1]; continue; }

  if [[ ${IGNORE_MD5} -ne 1 ]]; then
    md5sum --check --status ${md5file} < ${file} \
      || { echo "!!! md5sum doesn't match decompressed file";
           nogood=$[${nogood}+1]; continue; }
  fi

  # Compress and decompress a copy
  mv "${file}" "${copy}"
  rm -f "${bzcopy}"
  echo "  Recompress..."
  ${VALGRIND} ${VALGRIND_ARGS} ${BZIP2} -z -q -s ${copy} \
    || { echo "!!! bad compress result $?";
         nogood=$[${nogood}+1]; continue; }
  echo "  Redecompress..."
  ${VALGRIND} ${VALGRIND_ARGS} ${BZIP2} -d -q -s ${bzcopy} \
    || { echo "!!! bad (re)decompress result $?";
         nogood=$[${nogood}+1]; continue; }

  if [[ ${IGNORE_MD5} -ne 1 ]]; then
    md5sum --check --status ${md5file} < ${copy} \
      || { echo "!!! md5sum doesn't match (re)decompressed file";
           nogood=$[${nogood}+1]; continue; }
  fi

  rm "${copy}"

  # Now do it all again in "small" mode.
  echo "  Decompress (small)..."
  rm -f "${file}"
  ${VALGRIND} ${VALGRIND_ARGS} ${BZIP2} -k -d -q -s ${bzfile} \
    || { echo "!!! bad decompress result $?";
         nogood=$[${nogood}+1]; continue; }

  if [[ ${IGNORE_MD5} -ne 1 ]]; then
    md5sum --check --status ${md5file} < ${file} \
      || { echo "!!! md5sum doesn't match decompressed file";
           nogood=$[${nogood}+1]; continue; }
  fi

  # Compress and decompress a copy
  mv "${file}" "${copy}"
  rm -f "${bzcopy}"
  echo "  Recompress (small)..."
  ${VALGRIND} ${VALGRIND_ARGS} ${BZIP2} -z -q -s ${copy} \
    || { echo "!!! bad compress result $?";
         nogood=$[${nogood}+1]; continue; }
  echo "  Redecompress (small)..."
  ${VALGRIND} ${VALGRIND_ARGS} ${BZIP2} -d -q -s ${bzcopy} \
    || { echo "!!! bad (re)decompress result $?";
         nogood=$[${nogood}+1]; continue; }

  if [[ ${IGNORE_MD5} -ne 1 ]]; then
    md5sum --check --status ${md5file} < ${copy} \
      || { echo "!!! md5sum doesn't match (re)decompressed file";
           nogood=$[${nogood}+1]; continue; }
  fi

  rm "${copy}"

done < <(find ${TESTS_DIR} -type f -name \*\.bz2 -print0)

echo
if [[ ${nogood} -eq 0 ]]; then
  echo "All .bz files decompressed and recompressed correctly."
else
  echo "!!! ${nogood} .bz files did not decompressed/recompressed correctly."
fi
echo

# Then test all bad inputs.
echo "Testing detection of bad input data..."
echo

nobad=0
badbad=0
while IFS= read -r -d '' badfile; do

  echo "Processing ${badfile}"

  echo "  Trying to decompress..."
  ${VALGRIND} ${VALGRIND_ARGS} ${BZIP2} -k -d -q ${badfile}
  ret=$?

  if [[ ${ret} -eq 0 ]]; then
    echo "!!! badness not detected"
    nobad=$[${nobad}+1]
    continue
  fi

  # Assumes "normal" badness is detected by exit code 1 or 2.
  # A crash or valgrind issue will be reported with something else.
  if [[ ${ret} != 1 ]] && [[ ${ret} != 2 ]]; then
    echo "!!! baddness caused baddness in ${BZIP2}"
    badbad=$[${badbad}+1]
    continue
  fi

  echo "  Trying to decompress (small)..."
  ${VALGRIND} ${VALGRIND_ARGS} ${BZIP2} -k -d -q -s ${badfile}
  ret=$?

  if [[ ${ret} -eq 0 ]]; then
    echo "!!! badness not detected"
    nobad=$[${nobad}+1]
    continue
  fi

  # Assumes "normal" badness is detected by exit code 1 or 2.
  # A crash or valgrind issue will be reported with something else.
  if [[ ${ret} != 1 ]] && [[ ${ret} != 2 ]]; then
    echo "!!! baddness caused baddness in ${BZIP2}"
    badbad=$[${badbad}+1]
    continue
  fi

done < <(find ${TESTS_DIR} -type f -name \*\.bz2.bad -print0)

badresults=$[${nobad}+${badbad}]
echo
if [[ ${badresults} -eq 0 ]]; then
  echo "Correctly found all bad file data integrity errors."
else
  if [[ ${nobad} -ne 0 ]]; then
    echo "!!! ${nobad} files not correctly detected as bad."
  fi
  if [[ ${badbad} -ne 0 ]]; then
    echo "!!! ${badbad} files caused baddness."
  fi
fi
echo

results=$[${nogood}+${badresults}]
if [[ ${results} -eq 0 ]]; then
  echo "All tests passed"
  exit 0
else
  echo "Bad results, look for !!! in the logs above"
  exit 1
fi
