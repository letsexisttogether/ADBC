import subprocess
from pathlib import Path

buildDirectory = Path('Build');

if not buildDirectory.exists():
    print(f'Specified dir for build ({buildDirectory}) does not exist') 

result = subprocess.run(['cmake', '-G', 'Ninja', '-B', \
    buildDirectory, '-S' './', '-DCMAKE_BUILD_TYPE=Debug',
    '-DCMAKE_EXPORT_COMPILE_COMMANDS=ON'])
