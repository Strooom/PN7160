"""Run the real NCI activation handler using the existing generic host hooks."""
import os
from pathlib import Path
import shlex
import subprocess
import tempfile

root = Path(__file__).resolve().parents[2]
compiler = shlex.split(os.environ.get("CXX", "c++"))
sources = ["nci/nci.cpp", "nci/ncistate.cpp", "nci/ncipacket.cpp", "tag/tag.cpp",
           "logging/logging.cpp", "timer/singletimer.cpp",
           "pn7160config/pn7160config.cpp", "pn7160config/pn7160configcollection.cpp",
           "pn7160interface/pn7160interface.cpp"]
includes = ["-I" + str(path) for path in (root / "lib").iterdir() if path.is_dir()]
with tempfile.TemporaryDirectory(prefix="pn7160-activation-") as temp:
    for optimization in ("-O0", "-O2"):
        binary = str(Path(temp) / optimization[1:])
        subprocess.run([
            *compiler, "-std=c++17", optimization, "-UNDEBUG", "-Dgeneric", "-DunitTesting",
            "-fsanitize=address,undefined", *includes,
            str(root / "test/activation/activation_test.cpp"),
            *(str(root / "lib" / source) for source in sources), "-o", binary,
        ], check=True)
        subprocess.run([binary], check=True)
        print(f"{optimization}: passed", flush=True)
