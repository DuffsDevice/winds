#include <_dialog/dialog.file.delete.h>
#include <_controller/controller.localization.h>

_fileDeleteDialog::_fileDeleteDialog( _direntry fileToDelete ) :
	_imageDialog(
		fmt2string(
			fileToDelete.isDirectory() ?
				_localizationController::getBuiltInString("fmt_delete_folder").c_str()
				: _localizationController::getBuiltInString("fmt_delete_file").c_str()
			, fileToDelete.getFullName().c_str()
		)
		, _localizationController::getBuiltInString("lbl_delete")
		, fileToDelete.getFileImage()
		, _localizationController::getBuiltInString("lbl_delete")
		, _localizationController::getBuiltInString("lbl_cancel")
	)
	, fileToDelete( fileToDelete.getFileName() )
{}