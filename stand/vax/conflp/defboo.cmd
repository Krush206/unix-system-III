!
!	DEFAULT BOOT COMMAND FILE - DEFBOO.CMD
!
HALT			! HALT PROCESSOR
UNJAM			! UNJAM SBI
INIT			! INIT PROCESSOR
D/I 11 20003800	! SET UP SCBB
D R0 0		! DISK PACK DEVICE TYPE
D R1 8		! MBA TR=8
D R2 0		! ADAPTER UNIT = 0
D R3 0		! CONTROLLER UNIT = 0
D R4 0		! BOOT BLOCK LBN (UNUSED)
D R5 8		! SOFTWARE BOOT FLAGS
D FP 0		! SET NO MACHINE CHECK EXPECTED
START 20003000		! START ROM PROGRAM
WAIT DONE		! WAIT FOR COMPLETION
			! 
LOAD SASH	! LOAD PRIMARY BOOTSTRAP
START 2			! AND START IT
