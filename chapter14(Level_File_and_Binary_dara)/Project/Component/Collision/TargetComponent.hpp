class TargetComponent : public Component
{
public:
	TargetComponent(class Actor* owner);
	~TargetComponent();
	TypeID GetType() const override { return TTargetComponent; }
};