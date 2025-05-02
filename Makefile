all: exe01/main1
	@./exe01/main1
	# @./exe02/main2

exe01/main1:
	@gcc exe01/main.c exe01/user.c -o $@
