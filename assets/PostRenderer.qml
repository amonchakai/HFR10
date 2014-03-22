import bb.cascades 1.2

Container {
    Label {
        text: "<html><div>" + ListItemData.post + "</div></html>"
        textStyle.base: SystemDefaults.TextStyles.BodyText
        textStyle.fontSize: FontSize.Small
        multiline: true
    }
}

