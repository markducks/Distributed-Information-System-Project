import org.apache.tools.ant.taskdefs.condition.Os

plugins {
   id("cpp-application")
}

// task to run cpp program
tasks.register("run", Exec::class) {
    dependsOn("build")         // make sure project has been built
    group = "application"                // set task group
    //standardInput = System.`in`    // enable commandline input
    val exeDir: String = "${buildDir}/exe/main/debug/"
    var exeFile: String? = "Hello"

    /*
    var cmd:     String? = null
    var param:   String? = null
    when {
        Os.isFamily(Os.FAMILY_WINDOWS) -> {
            exeFile = "HelloWorld.exe"
            cmd     = "cmd"
            param   = "/k"
        }
        Os.isFamily(Os.FAMILY_MAC) -> {
            exeFile = "HelloWorld"
            cmd     = ""
            param   = ""
        }
        Os.isFamily(Os.FAMILY_UNIX) -> {
            exeFile = "HelloWorld"
            cmd     = ""
            param   = "" //parametri!!!
        }
        else -> { throw GradleException(":cpp:HelloWorld run-target -> unknown OS family encountered")}
    }*/

    commandLine(exeDir + exeFile) // start command in cmd shell
    }
