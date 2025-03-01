How to add components to our hands-emg-lib

1. Download the following from Ultra Librarian (https://app.ultralibrarian.com/)
	- Symbol (.kicad_sym)
	- Footprint (.kicad_mod) (Ignore the -L or -M footprints, use the normal one)
	- 3D Model (.stp or .step)
	* My typical download process
		* 3D CAD Model: Check STEP
		* KiCAD: Check KiCAD v6+
		* Symbol Pin Ordering: Designers preference, read the (i) to understand what this is.
		  Download both and compare to see which one would make a cleaner schematic.
		* Footprint Units: Metric (mm)
	
1b. If you cannot find the component on Ultra Librarian, check Component Search Engine (https://componentsearchengine.com/)
	- Don't mix and match symbols and footprints from different websites
	- If you cant find the components 3d model on ultralibrarian, its fine to download the 3d model from component search engine and attach to footprint from ultralibrarian.
	
1c. After downloading all the files it will be a zip do the following
	- Extract the zip to some folder on your PC.
	- Rename all the files to be the same name (typically part number)
	  Heres an example before and after with the ADS1299 from Ultra Librarian
			Before: 2025-03-01_06-35-12.kicad_sym
					PAG0064A.stp
					PAG64_TEX.kicad_mod
					
			After:  ADS1299-4PAGR.kicad_sym
					ADS1299-4PAGR.stp
					ADS1299-4PAGR.kicad_mod
	
2. Copy the 3d model file from the download into the hands-emg-lib.3dshapes.

3. Import the footprint
	- Open the KiCad Footprint Editor
	- Filter hands-emg-lib
	- Right click hands-emg-lib
	- Import Footprint
	- Select the footprint file from the download
	- Open the footprints properties menu
		- Check the "Value" and confirm its the proper manufacturer's part number (if you had to rename footprint file then it's likely incorrect)
		- Add a description of what it is (typically not part number, the description helps to search the part)
		- Go to the 3D model tab
		- Click the little folder icon to add 3d model
		- Select the 3d model file in the hands-emg-lib.3dshapes folder
		- Rotate/Offset model to fit the footprint (scaling is needed in very few cases)
	- Click OK to close the footprint properties menu
	- File -> Save
	- Dont change the name, filter for hands-emg-lib, click hands-emg-lib, click OK.
	
4. Import the symbol
	- Open the KiCad Symbol Editor
	- Filter hands-emg-lib
	- Right click hands-emg-lib
	- Import Symbol
	- Select the symbol file from the download
	- Open the symbol properties menu
		- Click on footprint
		- Click on the little books icon to open the "footprint chooser"
		- Filter for hands-emg-lib
		- Select the footprint imported in step 3 under hands-emg-lib (for this component)
		- Click OK
		- Add datasheet link under "Datasheet" field
		- Add description
		- Create new field
			- Click '+' symbol
			- Name this field procurement
			- Make sure to un-check the show column for this field
			- Provide purchase link for this component
				- Try to keep it all from DigiKey
				- No obsolete parts, no limited stock, must be immediately aviable, no lead time
		- Click OK to close symbol properties menu
	- File -> Save
	
5. Commit these changes to git and push so we all have easy access to the components.

6. Use your imported component. (Seach for hands-emg-lib in symbol/footprint selection menu when placing)

7. You can delete the downloaded files from your downloads folder, importing them has copied them all to our project.

Note! This library is 'project' scoped, there is no need to edit any of your kicad settings to use it. It will not be aviable in other projects.