SECTIONS
{
	.class.type ALIGN(8) :
	{
		PROVIDE(__class_type_start = .);
		KEEP(*(.class.type))
		PROVIDE(__class_type_end = .);
	}

	.class.table ALIGN(8) :
	{
		PROVIDE(__class_table_start = .);
		KEEP(*(.class.table))
		PROVIDE(__class_table_end = .);
	}
}