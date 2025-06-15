function Component() {}

Component.prototype.createOperations = function() {
    component.createOperations();

    var installPath = installer.value("TargetDir");
    var appName = installer.value("ProductName");
    var exePath = installPath + "/OxLauncher.exe";

    var baseUrl = "https://oxcorpimages.s3.us-east-1.amazonaws.com/launchers/zuno";
    var versionUrl = "https://oxcorpimages.s3.us-east-1.amazonaws.com/launchers/zuno/version.txt";

    component.addOperation("EnvironmentVariable", "OXCORP_INSTALL_PATH", installPath);
    component.addOperation("EnvironmentVariable", "OXCORP_EXECUTABLE", appName + ".exe");
    component.addOperation("EnvironmentVariable", "OXCORP_BASE_URL", baseUrl);
    component.addOperation("EnvironmentVariable", "OXCORP_VERSION_URL", versionUrl);

    component.addOperation("CreateShortcut",
        exePath,
        "@StartMenuDir@/" + appName + ".lnk",
        "workingDirectory=@TargetDir@",
        "iconPath=" + exePath
    );

    component.addOperation("CreateShortcut",
        exePath,
        "@DesktopDir@/" + appName + ".lnk",
        "workingDirectory=@TargetDir@",
        "iconPath=" + exePath
    );

    console.log("Environment variables set and shortcuts created.");
 }

