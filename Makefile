.PHONY: clean All

All:
	@echo "----------Building project:[ oper - Release ]----------"
	@"$(MAKE)" -f  "oper.mk"
clean:
	@echo "----------Cleaning project:[ oper - Release ]----------"
	@"$(MAKE)" -f  "oper.mk" clean
