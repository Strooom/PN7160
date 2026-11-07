# NFC-A activation validation regression

Run from the repository root:

```sh
python3 test/activation/run.py
```

The test compiles the actual NCI activation handler and its library dependencies
using the existing `generic` and `unitTesting` host hooks. It runs at `-O0` and
`-O2` with assertions enabled, AddressSanitizer and UndefinedBehaviorSanitizer.
No Arduino SDK, bus stubs or new transport implementation are needed.

Cases cover valid 4/7/10-byte UIDs, zero/one-byte SEL_RES, repeat detection,
every truncated prefix before the end of the technology-specific field,
invalid UID/technology lengths, unsupported technology mode, inconsistent NCI
payload length, and a SEL_RES byte extending past its declared field.

Each rejection case first installs a valid UID, then copies only the malformed
packet's actual bytes into the persistent receive buffer. Its untouched tail
therefore still contains the previous notification. Rejection must enter the
existing NCI error state, expose `tagStatus::absent`, and leave the stored UID
unchanged rather than interpreting the stale tail as a new tag.

Packets include the remaining activation fields, but these tests validate the
NFC-A UID/SEL_RES extraction boundary; they do not claim full semantic validation
of every NCI activation parameter or test physical RF/I2C behavior.

The regression fails on the original upstream implementation. It exercises the
handler directly, independently of transport buffering, and preserves the
project's existing CC BY-NC-SA 4.0 license.
