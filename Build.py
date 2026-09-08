import subprocess
from pathlib import Path

buildDirectory = Path('Build');

if not buildDirectory.exists():
    print(f'Specified dir for build ({buildDirectory}) does not exist') 

result = subprocess.run(['cmake', '--build', buildDirectory])
