pipeline {
	agent any
	stages {
		stage('SCM') {
			steps { 
				checkout scm
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
		stage("Check coverage"){
			steps{
				dir("Exercise/build") {
					sh 'make tests_coverage'
					publishCoverage adapters: [cobertura('test_coverage.xml')], sourceFileResolver: sourceFiles('NEVER_STORE')
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
