#include "WorldTransform.h"

void WorldTransform::UpdateMatrix()
{
	matWorld_ = Matrix4::Identity();
	matWorld_ *= Matrix4::Scaling(scale_.x, scale_.y, scale_.z);
	matWorld_ *= Matrix4::RotationZXY(rotation_.x, rotation_.y, rotation_.z);
	matWorld_ *= Matrix4::Translation(translation_.x, translation_.y, translation_.z);
	if (parent_ != nullptr) {
		matWorld_ *= parent_->matWorld_;
	}
	TransferMatrix();
}
