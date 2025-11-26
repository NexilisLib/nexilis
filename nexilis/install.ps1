Write-Host "Starting Nexilis installation..."

# Installation prompt for vcpkg
function Ask-VcpkgInstall {
    param()

    $answer = Read-Host "Do you want to install vcpkg package manager? (recommended for Visual Studio users) [Y/N]"

    if ($answer -match "^[Yy]$") {
        Write-Host "Installing vcpkg..."

        # Determine parent directory of the script location
        $scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
        $parentDir = Split-Path -Parent $scriptDir
        $vcpkgPath = Join-Path $parentDir "vcpkg"
        
        # Clone and bootstrap vcpkg
        git clone https://github.com/microsoft/vcpkg.git "$vcpkg_path"
        & "$vcpkg_path\bootstrap-vcpkg.bat"

        Write-Host "vcpkg installed successfully at: $vcpkg_path"
    }
    elseif ($answer -match "^[Nn]$") {
        Write-Host "Skipping installation."
    }
    else {
        Write-Host "Invalid input. Please enter Y or N."
        Ask-VcpkgInstall
    }
}

# Get number of CPU cores for parallel build
$cores = [Environment]::ProcessorCount

cmake -B build -DCMAKE_INSTALL_PREFIX="$PWD/build/install"
cmake --build build --target install -- -j$cores

Write-Host "Nexilis installed successfully to 'build/install' directory."

# Run vcpkg installation prompt
Ask-VcpkgInstall

Write-Host "Nexilis installation process complete!"