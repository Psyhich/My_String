pipeline {
	agent any
	stages {
		stage('SCM') {
			steps { 
				checkout scm
			}
		}
		stage("Check code") {
			steps {
				step([$class: 'MasterCoverageAction', scmVars: [GIT_URL: env.GIT_URL]])
			}
		}
		
		stage("Build") {
			steps {
				dir("Exercise/build") {
					sh 'cmake ../'
					sh 'make tests'
				}
			}
		}
		stage("Test") {
			steps {
				dir("Exercise/build") {
					sh 'ctest'
				}
			}
		}
		stage("Build main"){
			steps {
				dir("Exercise/build") {
					sh 'make main'
				}
				script {
					currentBuild.result = 'SUCCESS'
				}
			}
		}
	}
}
