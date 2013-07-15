import QtQuick 2.1
import QtQuick.Controls 1.0

ApplicationWindow {
	visible: true
	CodeEditor {
		id: centralEditor
		z: 1
		anchors.fill: parent
	}
	
	Rectangle {
		anchors.fill: centralEditor
		color: "red"
	}
}
