
pipeline{
    agent any
    
    environment {
        // 临时清空代理相关的环境变量
        HTTP_PROXY = ''
        HTTPS_PROXY = ''
        http_proxy = ''
        https_proxy = ''
        NO_PROXY = '*'
        
        // UE引擎位置
        UE_PATH = "D:\\UE4\\UnrealEngine-4.27.2-release" // 这里先用本地进行环境配置
    }
    
    parameters{
        choice(
            name: 'PLATFORM',
            choices: ['Win64', 'Android','IOS'],
            description: 'Chooise Platform'
        )

        choice(
            name: 'CONFIG',
            choices: ['Development', 'Shipping','Debug', 'Test'],
            description: 'Chooise package type'
        )
        // stages control
        booleanParam(
            name: 'DO_CLEAN',
            defaultValue: false,
            description: '是否执行clean清理'
        )
        booleanParam(name: 'DO_BUILD', defaultValue: true, description: '执行 Build')
        booleanParam(name: 'DO_COOK', defaultValue: true, description: '执行 Cook')
        booleanParam(name: 'DO_STAGE', defaultValue: true, description: '执行 Stage')
        booleanParam(name: 'DO_PAK', defaultValue: true, description: '执行 Package')
        booleanParam(name: 'DO_TEST', defaultValue: true, description: '执行 Test')

        // Package control
        booleanParam(
            name: 'BUILD_WIN64',
            defaultValue: true,
            description: '构建Win64'
        )
        booleanParam(
            name: 'BUILD_ANDROID',
            defaultValue: false,
            description: '构建Android'
        )
        booleanParam(
            name: 'BUILD_IOS',
            defaultValue: false,
            description: '构建IOS'
        )

        booleanParam(
            name: 'BUILD_DEVELOPMENT',
            defaultValue: true,
            description: '构建Development'
        )

        booleanParam(
            name: 'BUILD_SHIPPING',
            defaultValue: false,
            description: '构建Win64'
        )
        booleanParam(
            name: 'BUILD_TEST',
            defaultValue: false,
            description: '构建Test'
        )
           booleanParam(
            name: 'BUILD_DEBUG',
            defaultValue: false,
            description: '构建DEBUG'
        )
        
    }
    stages{
        stage('Check config'){
            steps{
                script{
                    echo 'Checking config....'
                    def TargetPlatform = []
                    if (params.BUILD_WIN64) TargetPlatform << 'Win64'
                    if (params.BUILD_ANDROID) TargetPlatform << 'Android'
                    if (params.BUILD_IOS) TargetPlatform << 'IOS'

                    def BuildConfig = []
                    if (params.BUILD_DEVELOPMENT) BuildConfig << 'Development'
                    if (params.BUILD_DEBUG) BuildConfig << 'Debug'
                    if (params.BUILD_SHIPPING) BuildConfig << 'Shipping'
                    if (params.BUILD_TEST) BuildConfig << 'Test'

                    if (!TargetPlatform){
                        error 'A Platform must be selected!'
                    }
                    if (!BuildConfig){
                        error 'A Config must be selected!'
                    }

                    // 拼接参数
                    env.PLATFORM_LIST = TargetPlatform.join('|')
                    env.CONFIG_LIST = BuildConfig.join('|')

                    // print
                    echo "Selected Platforms: ${env.PLATFORM_LIST}"
                    echo "Selected Configs: ${env.CONFIG_LIST}"

                }
               
            }
        }
        stage('Checkout project'){
            steps{
                echo 'Checking latest code'
                git(
                    url: 'git@github.com:AsuLinBeats/Unreal_Automation_Test.git',
                    credentialsId: 'Test1',
                    branch: 'master'
                )
                bat 'git lfs pull'
                
            }
        }
        stage('Clean'){
            when{
                expression {params.DO_CLEAN}
            }
            steps{
                bat'''
                echo 'Optional Stage: Clean intermediate file for rebuilding'
                set "UAT_PARAMS_CLEAN=BuildTarget -clean -target=KuroTask_AutoTest -project=%WORKSPACE%\\KuroTask_AutoTest.uproject -FromMSBuild -WaitMutex"
                
                call "%UE_PATH%\\Engine\\Build\\BatchFiles\\RunUAT.bat" %UAT_PARAMS_CLEAN%
                
                if errorlevel 1 (
                    echo UAT Clean执行失败
                    exit /b 1
                ) else (
                    echo UAT Clean执行成功
                )
                '''
            }
        }
        stage('Build'){
            when{
                expression {params.DO_BUILD}
            }
            steps{
                script{
                    def platforms = env.PLATFORM_LIST.split('\\|')
                    def configs = env.CONFIG_LIST.split('\\|')

                    def tasks = [:]
                    for(platform in platforms){
                        for(config in configs){
                            def p = platform.trim()
                            def c = config.trim()
                            def taskName = "Build_${p}_${c}"
                            
                            tasks[taskName] = {
                                bat """
                                    echo Building ${p} with ${c}
                                    set "UAT_PARAMS_BUILD=BuildTarget -target=KuroTask_AutoTest -project=%WORKSPACE%\\KuroTask_AutoTest.uproject -Platform=${p} -configuration=${c}"
                                    
                                    call "%UE_PATH%\\Engine\\Build\\BatchFiles\\RunUAT.bat" %UAT_PARAMS_BUILD%
                                    
                                    if errorlevel 1 (
                                        echo UAT Build执行失败
                                        exit /b 1
                                    ) else (
                                        echo UAT Build执行成功
                                    )
                                """
                            }

                        }
                    }
                    parallel tasks
                }
        
            }
        }
        
        stage('Cook'){
            when{
                expression {params.DO_COOK}
            }
            steps{
                script{
                    def platforms = env.PLATFORM_LIST.split('\\|')
                    def configs = env.CONFIG_LIST.split('\\|')

                    def tasks = [:]
                    for(platform in platforms){
                        for(config in configs){
                            def p = platform.trim()
                            def c = config.trim()
                            def taskName = "Cook_${p}_${c}"
                            
                            tasks[taskName] = {
                                bat """
                                    echo Cooking ${p} with ${c}
                                    set "UAT_PARAMS_COOK=BuildCookRun -project=%WORKSPACE%\\KuroTask_AutoTest.uproject -Platform=${p} -configuration=${c} -cook -skipbuild"
                                    
                                    call "%UE_PATH%\\Engine\\Build\\BatchFiles\\RunUAT.bat" %UAT_PARAMS_COOK%
                                    
                                    if errorlevel 1 (
                                        echo UAT Cook执行失败
                                        exit /b 1
                                    ) else (
                                        echo UAT Cook执行成功
                                    )
                                """
                            }



                        }
                    }
                    parallel tasks
                }
            }

        }
        stage('Staging'){
            when{
                expression {params.DO_STAGE}
            }

            steps{
                script{
                    def platforms = env.PLATFORM_LIST.split('\\|')
                    def configs = env.CONFIG_LIST.split('\\|')

                    def tasks = [:]
                    for(platform in platforms){
                        for(config in configs){
                            def p = platform.trim()
                            def c = config.trim()
                            def taskName = "Stage_${p}_${c}"
                            
                            tasks[taskName] = {
                                bat """
                                    echo Staging ${p} with ${c}
                                    set "UAT_PARAMS_STAGE=BuildCookRun -project=%WORKSPACE%\\KuroTask_AutoTest.uproject -Platform=${p} -configuration=${c} -stage -skipbuild -skipcook -skippak -skiparchive -noP4"
                                    
                                    call "%UE_PATH%\\Engine\\Build\\BatchFiles\\RunUAT.bat" %UAT_PARAMS_STAGE%
                                    
                                    if errorlevel 1 (
                                        echo UAT Stage执行失败
                                        exit /b 1
                                    ) else (
                                        echo UAT Stage执行成功
                                    )
                                """
                            }



                        }
                    }
                    parallel tasks
                }
            }

        }
        
        stage('Packaging'){
            when{
                expression {params.DO_PAK}
            }
            steps{
                script{
                    def platforms = env.PLATFORM_LIST.split('\\|')
                    def configs = env.CONFIG_LIST.split('\\|')

                    def tasks = [:]
                    for(platform in platforms){
                        for(config in configs){
                            def p = platform.trim()
                            def c = config.trim()
                            def taskName = "Package_${p}_${c}"
                            
                            tasks[taskName] = {
                                bat """
                                    echo Packaging ${p} with ${c}
                                    set "UAT_PARAMS_PAK=BuildCookRun -project=%WORKSPACE%\\KuroTask_AutoTest.uproject -Platform=${p} -configuration=${c} -pak -skipstage -noP4"
                                    
                                    call "%UE_PATH%\\Engine\\Build\\BatchFiles\\RunUAT.bat" %UAT_PARAMS_PAK%
                                    
                                    if errorlevel 1 (
                                        echo UAT Package执行失败
                                        exit /b 1
                                    ) else (
                                        echo UAT Package执行成功
                                    )
                                """
                            }
                        }
                    }
                    parallel tasks
                }
            }
        }
        stage('Testing'){
            steps{
                echo 'Testing'
                // run predefined texture test
                // TODO: 之后，Auto代码加上不同类型的test，然后这里检查commit的文件，根据类型调用不同的方法
                bat '''
                    echo Test Project via Auto Test System in ue4
                    
                    call "%UE_PATH%\\Engine\\Binaries\\Win64\\UE4Editor-cmd.exe" "%WORKSPACE%\KuroTask_AutoTest.uproject" -unattended -nop4 -nosplash -nullrhi -ExecCmds="Automation RunTests TextureChecker; Quit"  -testexit="Automation Test Queue Empty" -log
                '''
            }
        }
    }
}
