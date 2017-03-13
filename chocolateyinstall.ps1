$ErrorActionPreference = 'Stop';

$packageName= 'tome-editor'

$toolsDir   = "$(Split-Path -parent $MyInvocation.MyCommand.Definition)"
$exampleDir = "$(Join-Path -Path $toolsDir -ChildPath ..\example)"

$url        = 'https://github.com/npruehs/tome-editor/releases/download/0.9/Tome-0.9-Windows-x86.zip'
$urlExample= 'https://github.com/npruehs/tome-editor/releases/download/0.9/Tome-0.9-ExampleProjects.zip'

# Install Tome.
Install-ChocolateyZipPackage $packageName $url $toolsDir

# Install example project.
Install-ChocolateyZipPackage $packageName $urlExample $exampleDir
