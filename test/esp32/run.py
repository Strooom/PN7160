#!/usr/bin/env python3
"""Run the real ESP32 transport and shared NCI engine against fake hardware."""
import os
from pathlib import Path
import shlex
import subprocess
import tempfile

root = Path(__file__).resolve().parents[2]
test = root / "test/esp32"
compiler = shlex.split(os.environ.get("CXX", "c++"))
common = ["-std=c++17", "-O1", "-UNDEBUG", "-fsanitize=address,undefined", "-DunitTesting",
          "-I" + str(test / "stubs"), "-I" + str(root / "lib")]
transport = root / "lib/pn7160interface/pn7160interface.cpp"
engine = [root / "lib" / path for path in (
    "nci/nci.cpp", "nci/ncistate.cpp", "nci/ncipacket.cpp", "tag/tag.cpp",
    "logging/logging.cpp", "timer/singletimer.cpp", "pn7160config/pn7160config.cpp",
    "pn7160config/pn7160configcollection.cpp")]
with tempfile.TemporaryDirectory(prefix="pn7160-tests-") as directory:
    for name, sources in (("transport_test", [transport]), ("nci_tag_test", [transport, *engine]),
                          ("legacy_transport_test", [transport])):
        binary = Path(directory) / name
        arch = "-DPN7160_RP2040" if name == "legacy_transport_test" else "-DARDUINO_ARCH_ESP32"
        subprocess.run([*compiler, *common, arch, str(test / (name + ".cpp")),
                        *map(str, sources), "-o", str(binary)], check=True)
        subprocess.run([str(binary)], check=True)
