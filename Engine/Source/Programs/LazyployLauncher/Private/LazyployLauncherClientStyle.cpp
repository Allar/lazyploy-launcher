// Copyright 2016 Gamemakin LLC. All Rights Reserved.

#include "LazyployLauncherClientStyle.h"

TSharedPtr< FSlateStyleSet > FLazyployLauncherClientStyle::StyleSet = nullptr;

void FLazyployLauncherClientStyle::Initialize()
{
	if (!StyleSet.IsValid())
	{
		StyleSet = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleSet);
	}
}

void FLazyployLauncherClientStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet);
	StyleSet.Reset();
}

#define TTF_FONT(RelativePath, ...) FSlateFontInfo(ContentFromEngine(TEXT(RelativePath), TEXT(".ttf")), __VA_ARGS__)
#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush(ContentFromEngine(TEXT(RelativePath), TEXT(".png")), __VA_ARGS__)
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush(ContentFromEngine(TEXT(RelativePath), TEXT(".png")), __VA_ARGS__)
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush(ContentFromEngine(RelativePath, TEXT(".png")), __VA_ARGS__)

namespace
{
	FString ContentFromEngine(const FString& RelativePath, const TCHAR* Extension)
	{
		static const FString ContentDir = FPaths::EngineDir() / TEXT("Content/Slate");
		return ContentDir / RelativePath + Extension;
	}
}

TSharedRef< FSlateStyleSet > FLazyployLauncherClientStyle::Create()
{
	const FVector2D Icon16x16(16.0f, 16.0f);
	const FVector2D Icon32x32(32.0f, 32.0f);
	const FVector2D Icon48x48(48.0f, 48.0f);
	const FVector2D Icon64x64(64.0f, 64.0f);

	TSharedRef<FSlateStyleSet> StyleRef = MakeShareable(new FSlateStyleSet("FLazyployLauncherClientStyle"));
	FSlateStyleSet& Style = StyleRef.Get();

	const FTextBlockStyle DefaultText = FTextBlockStyle()
		.SetFont(TTF_FONT("Fonts/Roboto-Regular", 10))
		.SetColorAndOpacity(FSlateColor::UseForeground())
		.SetShadowOffset(FVector2D::ZeroVector)
		.SetShadowColorAndOpacity(FLinearColor::Black);

	// Set the client app styles
	Style.Set(TEXT("Background"), new IMAGE_BRUSH("Common/NoiseBackground", Icon64x64, FLinearColor(1,1,1,1), ESlateBrushTileType::Both));

	Style.Set(TEXT("ProjectPicker.Text"), FTextBlockStyle(DefaultText)
		.SetFont(TTF_FONT("Fonts/Roboto-Regular", 24))
		);
	
	Style.Set(TEXT("ProjectPicker.ErrorImage"), new IMAGE_BRUSH("Icons/icon_error_16x", Icon32x32));

	Style.Set(TEXT("Section.Label"), FTextBlockStyle(DefaultText)
		.SetFont(TTF_FONT("Fonts/Roboto-Bold", 18))
		);

	Style.Set(TEXT("Section.Border"), new BOX_BRUSH("Common/GroupBorder", FMargin(4.0f / 16.0f)));

	Style.Set(TEXT("Section.Text"), FTextBlockStyle(DefaultText)
		.SetFont(TTF_FONT("Fonts/Roboto-Regular", 10))
		);


	Style.Set(TEXT("Code"), FTextBlockStyle(DefaultText)
		.SetFont(TTF_FONT("Fonts/Roboto-Regular", 8))
		.SetColorAndOpacity(FSlateColor(FLinearColor::White * 0.8f))
		);

	Style.Set(TEXT("Title"), FTextBlockStyle(DefaultText)
		.SetFont(TTF_FONT("Fonts/Roboto-Bold", 12))
		);

	Style.Set(TEXT("Status"), FTextBlockStyle(DefaultText)
		.SetColorAndOpacity(FSlateColor::UseSubduedForeground())
		);

	FSlateBrush* GenericWhiteBox = new IMAGE_BRUSH("Old/White", Icon16x16);

	// Scrollbar
	const FScrollBarStyle ScrollBar = FScrollBarStyle()
		.SetVerticalTopSlotImage(IMAGE_BRUSH("Common/Scrollbar_Background_Vertical", FVector2D(8, 8)))
		.SetVerticalBottomSlotImage(IMAGE_BRUSH("Common/Scrollbar_Background_Vertical", FVector2D(8, 8)))
		.SetHorizontalTopSlotImage(IMAGE_BRUSH("Common/Scrollbar_Background_Horizontal", FVector2D(8, 8)))
		.SetHorizontalBottomSlotImage(IMAGE_BRUSH("Common/Scrollbar_Background_Horizontal", FVector2D(8, 8)))
		.SetNormalThumbImage(BOX_BRUSH("Common/Scrollbar_Thumb", FMargin(4.f / 16.f)))
		.SetDraggedThumbImage(BOX_BRUSH("Common/Scrollbar_Thumb", FMargin(4.f / 16.f)))
		.SetHoveredThumbImage(BOX_BRUSH("Common/Scrollbar_Thumb", FMargin(4.f / 16.f)));

	// SEditableTextBox defaults...
	const FEditableTextBoxStyle NormalEditableTextBoxStyle = FEditableTextBoxStyle()
		.SetBackgroundImageNormal(*GenericWhiteBox)
		.SetBackgroundImageHovered(*GenericWhiteBox)
		.SetBackgroundImageFocused(*GenericWhiteBox)
		.SetBackgroundImageReadOnly(*GenericWhiteBox)
		.SetScrollBarStyle(ScrollBar);
	{
		Style.Set("NormalEditableTextBox", NormalEditableTextBoxStyle);
	}

	// RichText
	const FTextBlockStyle LazyployLauncherDataStyle = FTextBlockStyle()
		.SetFont(TTF_FONT("Testing/Fonts/Roboto-Italic", 9))
		.SetColorAndOpacity(FSlateColor(FLinearColor::White * 0.5f))
		.SetShadowOffset(FVector2D::ZeroVector)
		.SetShadowColorAndOpacity(FLinearColor::Black);

	Style.Set("LazyployLauncherDataStyle", LazyployLauncherDataStyle);

	FButtonStyle DarkHyperlinkButton = FButtonStyle()
		.SetNormal(BORDER_BRUSH("Old/HyperlinkDotted", FMargin(0, 0, 0, 3 / 16.0f), FSlateColor(FLinearColor::White * 0.5f)))
		.SetPressed(FSlateNoResource())
		.SetHovered(BORDER_BRUSH("Old/HyperlinkUnderline", FMargin(0, 0, 0, 3 / 16.0f), FSlateColor(FLinearColor::White * 0.5f)));

	const FHyperlinkStyle DarkHyperlink = FHyperlinkStyle()
		.SetUnderlineStyle(DarkHyperlinkButton)
		.SetTextStyle(LazyployLauncherDataStyle)
		.SetPadding(FMargin(0.0f));

	Style.Set("RichText.Hyperlink", DarkHyperlink);

	return StyleRef;
}

#undef TTF_FONT

const ISlateStyle& FLazyployLauncherClientStyle::Get()
{
	return *StyleSet;
}

const TSharedPtr<FSlateStyleSet> FLazyployLauncherClientStyle::GetPtr()
{
	return StyleSet;
}
