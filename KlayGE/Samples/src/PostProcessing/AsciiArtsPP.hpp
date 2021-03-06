#ifndef _ASCIIARTSPP_HPP
#define _ASCIIARTSPP_HPP

#include <KlayGE/PreDeclare.hpp>
#include <KlayGE/PostProcess.hpp>

class AsciiArtsPostProcess : public KlayGE::PostProcess
{
public:
	AsciiArtsPostProcess();

	void InputPin(KlayGE::uint32_t index, KlayGE::TexturePtr const & tex);
	KlayGE::TexturePtr const & InputPin(KlayGE::uint32_t index) const;

	void Apply();

private:
	KlayGE::PostProcessPtr downsampler_;
	KlayGE::TexturePtr downsample_tex_;

	KlayGE::RenderEffectParameterPtr cell_per_row_line_ep_;
};

#endif		// _ASCIIARTSPP_HPP
