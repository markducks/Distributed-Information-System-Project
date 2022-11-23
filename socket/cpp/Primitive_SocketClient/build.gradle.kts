import org.apache.tools.ant.taskdefs.condition.Os

plugins {
   id("cpp-application")
}

// task to run cpp program
tasks.register("run", Exec::class) {
    dependsOn("build")         // make sure project has been built
    group = "application"                // set task group
    standardInput = System.`in`    // enable commandline input
    val exeDir: String = "${buildDir}/exe/main/debug/"
    var exeFile: String? = "Primitive_SocketClient"
    var param:   String? = "4000"

    commandLine(exeDir + exeFile) // start command in cmd shell
    }

