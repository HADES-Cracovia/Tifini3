##############################################################################
#
#  Makefile for Hydra library libHydra.so and libHydra.a
#
#  This makefile contains all definitions local to this module. All
#  general definitions are included from makefiles named "hades.*.mk".
#
##############################################################################

LIB_NAME := Tifini

USES_RFIO ?= no

SOURCE_FILES :=		KTifiniAnalysis.cc		\
			KAbstractAnalysis.cc		\
			KTrackInspector.cc		\
			KFitTools.cc			\
			KTools.cc

CPP_FLAGS += -std=c++0x

include $(HADDIR)/hades.def.mk
include $(HADDIR)/hades.module.mk
