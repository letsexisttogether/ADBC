import subprocess
from pathlib import Path

buildDirectory = Path('Build');

if not buildDirectory.exists():
    print(f'Specified dir for build ({buildDirectory}) does not exist') 

debugReleaseMode = 'Debug' if True else 'Release'

result = subprocess.run(['cmake', '--build', buildDirectory])
