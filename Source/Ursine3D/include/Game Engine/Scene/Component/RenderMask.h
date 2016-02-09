#pragma once

namespace ursine
{
    namespace ecs
    {
		enum class RenderMask : unsigned
		{
			Any = 0,
			M0 = 1 << 0,
			M1 = 1 << 1,
			M2 = 1 << 2,
			M3 = 1 << 3,
			M4 = 1 << 4,
			M5 = 1 << 5,
			M6 = 1 << 6,
			M7 = 1 << 7,
			M8 = 1 << 8,
			M9 = 1 << 9,
			Meta(Disable)
			MEditorTool = 1 << 10
		} Meta(Enable);

		namespace render_mask_t
		{
			using T = std::underlying_type_t<RenderMask>;
		}

		inline RenderMask operator|(const RenderMask &lhs, const RenderMask &rhs)
		{
			using namespace render_mask_t;

			return static_cast<RenderMask>( static_cast<T>( lhs ) | static_cast<T>( rhs ) );
		}

		inline const RenderMask &operator|=(RenderMask &lhs, const RenderMask &rhs)
		{
			using namespace render_mask_t;

			lhs = static_cast<RenderMask>( static_cast<T>( lhs ) | static_cast<T>( rhs ) );

			return lhs;
		}

		inline RenderMask operator&(const RenderMask &lhs, const RenderMask &rhs)
		{
			using namespace render_mask_t;

			return static_cast<RenderMask>( static_cast<T>( lhs ) & static_cast<T>( rhs ) );
		}

		inline const RenderMask &operator&=(RenderMask &lhs, const RenderMask &rhs)
		{
			using namespace render_mask_t;

			lhs = static_cast<RenderMask>( static_cast<T>( lhs ) & static_cast<T>( rhs ) );

			return lhs;
		}

		inline RenderMask operator~(const RenderMask &rhs)
		{
			using namespace render_mask_t;

			return static_cast<RenderMask>( static_cast<T>( rhs ) );
		}

		inline const RenderMask &operator<<(RenderMask &lhs, const RenderMask &rhs)
		{
			using namespace render_mask_t;

			return static_cast<RenderMask>( static_cast<T>( lhs ) << static_cast<T>( rhs ) );
		}
    }
}
