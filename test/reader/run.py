"""Test the actual reader, NCI engine and both transport platform branches."""
from pathlib import Path
import os
import shlex
import subprocess
import tempfile

root = Path(__file__).resolve().parents[2]
sources = ["reader/pn7160reader.cpp", "pn7160interface/pn7160interface.cpp",
           "nci/nci.cpp", "nci/ncistate.cpp", "nci/ncipacket.cpp", "tag/tag.cpp",
           "logging/logging.cpp", "timer/singletimer.cpp",
           "pn7160config/pn7160config.cpp", "pn7160config/pn7160configcollection.cpp"]
with tempfile.TemporaryDirectory(prefix="pn7160-reader-") as temp:
    for platform in ("ARDUINO_ARCH_ESP32", "PN7160_RP2040"):
        binary = str(Path(temp) / platform)
        subprocess.run([
            *shlex.split(os.environ.get("CXX", "c++")), "-std=c++17", "-O1", "-UNDEBUG",
            "-fsanitize=address,undefined", "-DunitTesting", "-D" + platform,
            "-I" + str(root / "test/esp32/stubs"), "-I" + str(root / "lib"),
            str(root / "test/reader/reader_test.cpp"),
            *(str(root / "lib" / source) for source in sources), "-o", binary,
        ], check=True)
        subprocess.run([binary], check=True)
        print(f"{platform}: passed", flush=True)
