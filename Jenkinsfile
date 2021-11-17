pipeline {
	agent any
	stages {
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
