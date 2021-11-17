pipeline {
	agent any
	stages {
		stage('SCM') {
			checkout scm
		}
		stage("Check code") {
			def scannerHome = tool 'SonarScanner';
			withSonarQubeEnv() {
					sh "${scannerHome}/bin/sonar-scanner"
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
			}
		}
	}
}
