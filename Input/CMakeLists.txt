PROJECT(Input)

SET(SOURCE_FILES
		Input.h
		ReadVectorMethod.h
		BaseSettings.h
		MatrixSettings.h
		VectorSettings.h
		Constant.h
		StreamString.h)

ADD_LIBRARY(Input SHARED ${SOURCE_FILES})
SET_TARGET_PROPERTIES(Input PROPERTIES LINKER_LANGUAGE CXX)
