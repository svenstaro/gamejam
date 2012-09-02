TextActor = gamvas.Actor.extend(
{
    create: function(name, text, x, y, size, color)
	{
		this._super(name, x, y);
		this.text = text;
        this.scale = typeof scale !== 'undefined' ? scale : 1;
        this.color = typeof color !== 'undefined' ? color : '#fff';
        size = typeof size !== 'undefined' ? size : 16;
		this.font = 'bold ' + size + 'px sans-serif';
	},
	
	draw: function(t)
	{
		var st = gamvas.state.getCurrentState();
        st.c.fillStyle = this.color;
        st.c.font = this.font;
		st.c.fillText(this.text, this.position.x, this.position.y);
	}
});

LevelName = TextActor.extend(
{
    create: function(text)
	{
		this._super("levelname", text, 120, 593);
	}
});
