import subprocess
from pathlib import Path

testDirectory = Path('Build')

if not testDirectory.exists():
    print(f'Specified dir for test ({testDirectory}) does not exist') 

result = subprocess.run(['ctest', '--test-dir', testDirectory])
